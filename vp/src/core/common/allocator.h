#pragma once

#include "tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h"

// **************************************************************************************
// User-defined memory manager, which maintains a pool of transactions
// **************************************************************************************

template <typename T>
class TransAllocator : public tlm::tlm_mm_interface {
   public:
	TransAllocator() : free_list(0), empties(0) {}

	T* allocate();
	void free(tlm::tlm_generic_payload* trans) override;

   private:
	struct access {
		T* trans;
		access* next;
		access* prev;
	};

	access* free_list;
	access* empties;
};

template <typename T>
inline T* TransAllocator<T>::allocate() {
	T* ptr;
	if (free_list) {
		ptr = free_list->trans;
		empties = free_list;
		free_list = free_list->next;
	} else {
		ptr = new T(this);
	}
	return ptr;
}

template <typename T>
inline void TransAllocator<T>::free(tlm::tlm_generic_payload* trans) {
	if (!empties) {
		empties = new access;
		empties->next = free_list;
		empties->prev = 0;
		if (free_list)
			free_list->prev = empties;
	}
	free_list = empties;
	free_list->trans = (T*)trans;
	empties = free_list->prev;
}

template <typename T>
class Transaction : public tlm::tlm_generic_payload {
   public:
	Transaction(tlm::tlm_mm_interface* mm) : tlm::tlm_generic_payload(mm) {}

	T get_data_copy() {
		return data;
	}

	T& get_data_ref() {
		return data;
	}

	T* get_data_ptr() {
		return &data;
	}

   private:
    // disallow copy and assignment
    Transaction(const Transaction& other) = delete;
    Transaction& operator=(const Transaction& other) = delete;
    
	T data;
};