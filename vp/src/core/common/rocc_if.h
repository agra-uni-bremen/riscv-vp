#pragma once

#include <string>

#include "instr.h"

#define ROCC_START_ADDR 0x10000000
#define ROCC_END_ADDR 0x1000ffff

#ifdef RV32B
using reg_t = uint32_t;
#else
using reg_t = uint64_t;
#endif

struct RoccInstruction : public Instruction {
	RoccInstruction() : Instruction() {}
	RoccInstruction(uint32_t instr) : Instruction(instr) {}

	inline bool xd() {
		return BIT_SINGLE(instr, 14);
	}

	inline bool xs1() {
		return BIT_SINGLE(instr, 13);
	}

	inline bool xs2() {
		return BIT_SINGLE(instr, 12);
	}
};

struct RoccCmd {
	RoccInstruction instr;
	reg_t rs1;
	reg_t rs2;
};

struct RoccResp {
	uint32_t rd;
	reg_t data;
};

struct TLA {
	uint32_t opcode;
	uint32_t param;
	uint32_t size;
	uint32_t source;
	uint32_t address;
	// user
	// echo
	uint32_t mask;
	std::string data;  // NOTE: Using hex string when value > 64 bits
	bool corrupt;
};

struct TLD {
	uint32_t opcode;
	uint32_t param;
	uint32_t size;
	uint32_t source;
	uint32_t sink;
	bool denied;
	// user
	// echo
	std::string data;  // NOTE: Using hex string when value > 64 bits
	bool corrupt;
};

struct RoccMemResp {};

struct rocc_if {
	virtual ~rocc_if() {}
};