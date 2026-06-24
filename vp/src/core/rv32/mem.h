#pragma once

#include "core/common/dmi.h"
#include "iss.h"
#include "mmu.h"

#include <tlm_utils/payload_extension.h>

namespace rv32 {

/* For optimization, use DMI to fetch instructions */
struct InstrMemoryProxy : public instr_memory_if {
	MemoryDMI dmi;

	tlm_utils::tlm_quantumkeeper &quantum_keeper;
	sc_core::sc_time clock_cycle = sc_core::sc_time(10, sc_core::SC_NS);
	sc_core::sc_time access_delay = clock_cycle * 2;

	InstrMemoryProxy(const MemoryDMI &dmi, ISS &owner) : dmi(dmi), quantum_keeper(owner.quantum_keeper) {}

	uint32_t load_instr(uint64_t pc) override { //spandan: removed virtual to stop replacement
		quantum_keeper.inc(access_delay);
		return dmi.load<uint32_t>(pc);
	}
};

struct CombinedMemoryInterface : public sc_core::sc_module,
                                 public instr_memory_if,
                                 public data_memory_if,
                                 public mmu_memory_if  {
	ISS &iss;
	std::shared_ptr<bus_lock_if> bus_lock;
	uint64_t lr_addr = 0;

	tlm_utils::simple_initiator_socket<CombinedMemoryInterface> isock;
	tlm_utils::tlm_quantumkeeper &quantum_keeper;

	// optionally add DMI ranges for optimization
	sc_core::sc_time clock_cycle = sc_core::sc_time(10, sc_core::SC_NS);
	sc_core::sc_time dmi_access_delay = clock_cycle * 4;
	std::vector<MemoryDMI> dmi_ranges;

    MMU *mmu;

	CombinedMemoryInterface(sc_core::sc_module_name, ISS &owner, MMU *mmu = nullptr)
	    : iss(owner), quantum_keeper(iss.quantum_keeper), mmu(mmu) {
	}

    uint64_t v2p(uint64_t vaddr, MemoryAccessType type) override {
	    if (mmu == nullptr)
	        return vaddr;
        return mmu->translate_virtual_to_physical_addr(vaddr, type);
    }

	inline void _do_transaction(tlm::tlm_command cmd, uint64_t addr, uint8_t *data, unsigned num_bytes, \
								tlm_utils::Type datatype = tlm_utils::Type::DATA) //spandan
	{
		tlm::tlm_generic_payload trans;
		trans.set_command(cmd);
		trans.set_address(addr);
		trans.set_data_ptr(data);
		trans.set_data_length(num_bytes);
		trans.set_response_status(tlm::TLM_OK_RESPONSE);

		/*spandan-------------------------------*/
		auto* ext = new tlm_utils::PayloadExtension();
		if (datatype != tlm_utils::Type::DATA) ext->payload_type = datatype;
		trans.set_extension(ext);
		/*spandan-------------------------------*/

		sc_core::sc_time local_delay = quantum_keeper.get_local_time();

		isock->b_transport(trans, local_delay);

		assert(local_delay >= quantum_keeper.get_local_time());
		quantum_keeper.set(local_delay);

		if (trans.is_response_error()) {
			if (iss.trace || iss.sys) {	// if iss has syscall interface, it likely has no traphandler for this
				uint64_t actual_addr = addr;
#ifdef TESTRIG
				actual_addr -= 0x80000000; // spandan: for TestRIG, out of scope addresses are added by 0x80000000
#endif 
				std::cout << "WARNING: core memory transaction failed for address 0x" << std::hex << actual_addr << std::dec << std::endl;
				if (cmd == tlm::TLM_READ_COMMAND)
					raise_trap(EXC_LOAD_ACCESS_FAULT, actual_addr); //spandan: for memory load fault
				else if (cmd == tlm::TLM_WRITE_COMMAND)
					raise_trap(EXC_STORE_AMO_ACCESS_FAULT, actual_addr); //spandan: for memory store fault
				else
					throw std::runtime_error("TLM command must be read or write");
			}
			if (cmd == tlm::TLM_READ_COMMAND)
				raise_trap(EXC_LOAD_PAGE_FAULT, addr);
			else if (cmd == tlm::TLM_WRITE_COMMAND)
				raise_trap(EXC_STORE_AMO_PAGE_FAULT, addr);
			else
				throw std::runtime_error("TLM command must be read or write");
		}
	}

/*spandan----------------------------------------------------------*/
	//Reset memory for TESTRIG
	inline void reset_mem() {
		_do_transaction(tlm::TLM_WRITE_COMMAND, 0, nullptr, 0, tlm_utils::Type::RESET);
	}

	bool get_tag(uint64_t addr) {
		uint8_t tag_byte;
		_do_transaction(tlm::TLM_READ_COMMAND, addr, (uint8_t *)&tag_byte, sizeof(uint8_t), tlm_utils::Type::TAG);
		uint64_t tag_bit_addr = addr / (BYTES_PER_TAG);
		uint8_t tag_byte_offset = tag_bit_addr % 8;
		uint8_t tag_bit_mask = (0b1 << tag_byte_offset);
		if (tag_byte & tag_bit_mask) return true;
		else return false;
	}

	void set_tag(uint64_t addr, bool tag) {
		uint8_t tag_byte, tag_bit_mask;

		uint64_t tag_bit_addr = addr / (BYTES_PER_TAG);
		uint8_t tag_byte_offset = tag_bit_addr%8;
		
		if (tag) { //if tag is 1 mask with or
			tag_bit_mask = (0b1 << tag_byte_offset);
			tag_byte = 0;
			_do_transaction(tlm::TLM_READ_COMMAND, addr, (uint8_t *)&tag_byte, sizeof(uint8_t), tlm_utils::Type::TAG);
			tag_byte |= tag_bit_mask;
			_do_transaction(tlm::TLM_WRITE_COMMAND, addr, (uint8_t *)&tag_byte, sizeof(uint8_t), tlm_utils::Type::TAG);
		}
		else { //if tag is 0 mask with and
			tag_bit_mask = ~(0b1 << tag_byte_offset);
			tag_byte = 0;
			_do_transaction(tlm::TLM_READ_COMMAND, addr, (uint8_t *)&tag_byte, sizeof(uint8_t), tlm_utils::Type::TAG);
			tag_byte &= tag_bit_mask;
			_do_transaction(tlm::TLM_WRITE_COMMAND, addr, (uint8_t *)&tag_byte, sizeof(uint8_t), tlm_utils::Type::TAG);
		}
	}

	template <typename T>
	inline T _raw_load_tags(uint64_t addr) {
		// NOTE: a DMI load will not context switch (SystemC) and not modify the memory, hence should be able to
		// postpone the lock after the dmi access
		bus_lock->wait_for_access_rights(iss.get_hart_id());

		for (auto &e : dmi_ranges) {
			if (e.contains(addr)) {
				quantum_keeper.inc(dmi_access_delay);
				return e.load_tags<T>(addr);
			}
		}

		//assert( 0 && "TLM transactions not implemented yet" ); //spandan
		//Check if address is aligned and the last address is within the scope of the memory
		uint32_t cap_addr;
		uint64_t end_addr = addr + (BYTES_PER_TAG)*(8*sizeof(T) - 1);
		_do_transaction(tlm::TLM_READ_COMMAND, end_addr, (uint8_t *)&cap_addr, sizeof(uint32_t), tlm_utils::Type::CAPABILITY);

		uint8_t copy_size = sizeof(T) + 1;
		T mask = ~0;

		uint64_t tag_bit_addr = addr / (BYTES_PER_TAG);
		uint64_t tag_byte_addr = tag_bit_addr/8;
		uint8_t tag_byte_offset = tag_bit_addr%8;

		static_assert(std::is_integral<T>::value, "integer type required");
		uint64_t ans = 0;
		_do_transaction(tlm::TLM_READ_COMMAND, tag_byte_addr, (uint8_t *)&ans, copy_size, tlm_utils::Type::TAG);

		T final_ans = static_cast<T>((ans >> tag_byte_offset) & mask);

		return final_ans;
	}

	template <typename T>
	inline T _raw_load_capability(uint64_t addr) {
		// NOTE: a DMI load will not context switch (SystemC) and not modify the memory, hence should be able to
		// postpone the lock after the dmi access
		bus_lock->wait_for_access_rights(iss.get_hart_id());

		for (auto &e : dmi_ranges) {
			if (e.contains(addr)) {
				quantum_keeper.inc(dmi_access_delay);
				return e.load_capability<T>(addr);
			}
		}

		//assert( 0 && "TLM transactions not implemented yet" ); //spandan
		T ans;
		uint64_t total_cap; //works for 32 bit capability only
			
		/*modified for parallel computation-----------------------------*/
		_do_transaction(tlm::TLM_READ_COMMAND, addr, (uint8_t *)&(total_cap), sizeof(total_cap), 
						tlm_utils::Type::CAPABILITY);
		/*modified for parallel computation-----------------------------*/
			
		ans.addr = total_cap & 0xFFFFFFFF;
		ans.caps = (total_cap >> (8*sizeof(ans.addr))) & 0xFFFFFFFF;
		ans.tag = get_tag(addr);

		/*
		T ans;
		uint8_t field_offset = 0;
		
		_do_transaction(tlm::TLM_READ_COMMAND, (addr+field_offset), (uint8_t *)&(ans.addr), sizeof(ans.addr), 
						tlm_utils::Type::CAPABILITY);
		field_offset += sizeof(ans.addr);
		_do_transaction(tlm::TLM_READ_COMMAND, (addr+field_offset), (uint8_t *)&(ans.caps), sizeof(ans.caps), 
						tlm_utils::Type::DATA);

		ans.tag = get_tag(addr);
		*/

		return ans;
	}

	template <typename T>
	inline void _raw_store_capability(uint64_t addr, T cs) {
		bus_lock->wait_for_access_rights(iss.get_hart_id());

		bool done = false;
		for (auto &e : dmi_ranges) {
			if (e.contains(addr)) {
				quantum_keeper.inc(dmi_access_delay);
				e.store_capability(addr, cs);
				done = true;
			}
		}

		if (!done) {
			//assert( 0 && "TLM transactions not implemented yet" ); //spandan
			uint64_t total_cap = 0; //works for 32 bit capability only
			total_cap = (static_cast<uint64_t>(cs.caps) << (8*sizeof(cs.addr))) | (static_cast<uint64_t>(cs.addr));
			
			/*modified for parallel computation-----------------------------*/
			_do_transaction(tlm::TLM_WRITE_COMMAND, addr, (uint8_t *)&(total_cap), sizeof(total_cap), 
						tlm_utils::Type::CAPABILITY);
			/*modified for parallel computation-----------------------------*/
			
			set_tag(addr, cs.tag);

			/*
			uint8_t field_offset = 0;

			_do_transaction(tlm::TLM_WRITE_COMMAND, (addr+field_offset), (uint8_t *)&(cs.addr), sizeof(cs.addr), 
						tlm_utils::Type::CAPABILITY);
			field_offset += sizeof(cs.addr);
			_do_transaction(tlm::TLM_WRITE_COMMAND, (addr+field_offset), (uint8_t *)&(cs.caps), sizeof(cs.caps), 
						tlm_utils::Type::DATA);
			
			set_tag(addr, cs.tag);
			*/
		}
		atomic_unlock();
	}
/*spandan----------------------------------------------------------*/

	template <typename T>
	inline T _raw_load_data(uint64_t addr) {
		// NOTE: a DMI load will not context switch (SystemC) and not modify the memory, hence should be able to
		// postpone the lock after the dmi access
		bus_lock->wait_for_access_rights(iss.get_hart_id());

		for (auto &e : dmi_ranges) {
			if (e.contains(addr)) {
				quantum_keeper.inc(dmi_access_delay);
				return e.load<T>(addr);
			}
		}

		//assert( 0 && "TLM transactions not implemented yet" ); //spandan
		T ans;
		_do_transaction(tlm::TLM_READ_COMMAND, addr, (uint8_t *)&ans, sizeof(T));
		return ans;
	}

	template <typename T>
	inline void _raw_store_data(uint64_t addr, T value) {
		bus_lock->wait_for_access_rights(iss.get_hart_id());

		bool done = false;
		for (auto &e : dmi_ranges) {
			if (e.contains(addr)) {
				quantum_keeper.inc(dmi_access_delay);
				e.store(addr, value);
				done = true;
			}
		}

		if (!done) {
			//assert( 0 && "TLM transactions not implemented yet" ); //spandan
			_do_transaction(tlm::TLM_WRITE_COMMAND, addr, (uint8_t *)&value, sizeof(T));
		}
		atomic_unlock();
	}

/*spandan-----------------------------------------*/
	template <typename T>
    inline T _load_tags(uint64_t addr) {
        return _raw_load_tags<T>(v2p(addr, LOAD));
    }

	template <typename T>
    inline T _load_capability(uint64_t addr) {
        return _raw_load_capability<T>(v2p(addr, LOAD));
    }

	template <typename T>
    inline void _store_capability(uint64_t addr, T cs) {
        _raw_store_capability(v2p(addr, STORE), cs);
    }
/*spandan-----------------------------------------*/

    template <typename T>
    inline T _load_data(uint64_t addr) {
        return _raw_load_data<T>(v2p(addr, LOAD));
    }

    template <typename T>
    inline void _store_data(uint64_t addr, T value) {
        _raw_store_data(v2p(addr, STORE), value);
    }

    uint64_t mmu_load_pte64(uint64_t addr) override {
        return _raw_load_data<uint64_t>(addr);
    }
    uint64_t mmu_load_pte32(uint64_t addr) override {
        return _raw_load_data<uint32_t>(addr);
    }
    void mmu_store_pte32(uint64_t addr, uint32_t value) override {
        _raw_store_data(addr, value);
    }

    void flush_tlb() override {
        mmu->flush_tlb();
    }

/*spandan-------------------------------------------*/
    uint32_t load_instr(uint64_t addr) override {
		uint32_t ans;
		uint64_t vaddr = v2p(addr, FETCH);
		_do_transaction(tlm::TLM_READ_COMMAND, vaddr, (uint8_t *)&ans, sizeof(uint32_t));
		return ans;
    }
/*spandan-------------------------------------------*/

    int64_t load_double(uint64_t addr) override {
        return _load_data<int64_t>(addr);
    }
	int32_t load_word(uint64_t addr) override {
		return _load_data<int32_t>(addr);
	}
	int32_t load_half(uint64_t addr) override {
		return _load_data<int16_t>(addr);
	}
	int32_t load_byte(uint64_t addr) override {
		return _load_data<int8_t>(addr);
	}
	uint32_t load_uhalf(uint64_t addr) override {
		return _load_data<uint16_t>(addr);
	}
	uint32_t load_ubyte(uint64_t addr) override {
		return _load_data<uint8_t>(addr);
	}
/*spandan--------------------------------------------*/
	Capability load_capability(uint64_t addr) override {
		return _load_capability<Capability>(addr);
	}
	uint32_t load_tags(uint64_t addr) override {
		return _load_tags<uint32_t>(addr);
	}
	void store_capability(uint64_t addr, Capability cs) override {
		_store_capability(addr, cs);
	}
/*spandan--------------------------------------------*/

    void store_double(uint64_t addr, uint64_t value) override {
        _store_data(addr, value);
    }
	void store_word(uint64_t addr, uint32_t value) override {
		_store_data(addr, value);
	}
	void store_half(uint64_t addr, uint16_t value) override {
		_store_data(addr, value);
	}
	void store_byte(uint64_t addr, uint8_t value) override {
		_store_data(addr, value);
	}

	virtual int32_t atomic_load_word(uint64_t addr) override {
		bus_lock->lock(iss.get_hart_id());
		return load_word(addr);
	}
	virtual void atomic_store_word(uint64_t addr, uint32_t value) override {
		assert(bus_lock->is_locked(iss.get_hart_id()));
		store_word(addr, value);
	}
	virtual int32_t atomic_load_reserved_word(uint64_t addr) override {
		bus_lock->lock(iss.get_hart_id());
		lr_addr = addr;
		return load_word(addr);
	}
	virtual bool atomic_store_conditional_word(uint64_t addr, uint32_t value) override {
		/* According to the RISC-V ISA, an implementation can fail each LR/SC sequence that does not satisfy the forward
		 * progress semantic.
		 * The lock is established by the LR instruction and the lock is kept while forward progress is maintained. */
		if (bus_lock->is_locked(iss.get_hart_id())) {
			if (addr == lr_addr) {
				store_word(addr, value);
				return true;
			}
			atomic_unlock();
		}
		return false;
	}
	virtual void atomic_unlock() override {
		bus_lock->unlock(iss.get_hart_id());
	}
};

}  // namespace rv32
