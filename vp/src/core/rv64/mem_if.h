#pragma once

#include <stdint.h>

namespace rv64 {

struct instr_memory_if {
	virtual ~instr_memory_if() {}

	virtual uint32_t load_instr(uint64_t pc) = 0;
};

struct mmu_memory_if {
	virtual ~mmu_memory_if() {}

	virtual uint64_t mmu_load_pte64(uint64_t addr) = 0;
	virtual uint64_t mmu_load_pte32(uint64_t addr) = 0;
	virtual void mmu_store_pte32(uint64_t addr, uint32_t value) = 0;
};

struct data_memory_if {
	virtual ~data_memory_if() {}

	virtual int64_t load_double(uint64_t addr) = 0;
	virtual int64_t load_word(uint64_t addr) = 0;
	virtual int64_t load_half(uint64_t addr) = 0;
	virtual int64_t load_byte(uint64_t addr) = 0;
	virtual uint64_t load_uword(uint64_t addr) = 0;
	virtual uint64_t load_uhalf(uint64_t addr) = 0;
	virtual uint64_t load_ubyte(uint64_t addr) = 0;

	virtual void store_double(uint64_t addr, uint64_t value) = 0;
	virtual void store_word(uint64_t addr, uint32_t value) = 0;
	virtual void store_half(uint64_t addr, uint16_t value) = 0;
	virtual void store_byte(uint64_t addr, uint8_t value) = 0;

	virtual int64_t atomic_load_word(uint64_t addr) = 0;
	virtual void atomic_store_word(uint64_t addr, uint32_t value) = 0;
	virtual int64_t atomic_load_reserved_word(uint64_t addr) = 0;
	virtual bool atomic_store_conditional_word(uint64_t addr, uint32_t value) = 0;
	virtual void atomic_unlock() = 0;

	virtual int64_t atomic_load_double(uint64_t addr) = 0;
	virtual void atomic_store_double(uint64_t addr, uint64_t value) = 0;
	virtual int64_t atomic_load_reserved_double(uint64_t addr) = 0;
	virtual bool atomic_store_conditional_double(uint64_t addr, uint64_t value) = 0;

	virtual void flush_tlb() = 0;
};

}  // namespace rv64