#ifndef RISCV_ISA_INSTR_H
#define RISCV_ISA_INSTR_H

#include <stdint.h>
#include <array>
#include <iostream>

#include "core_defs.h"

namespace Opcode {
// opcode masks used to decode an instruction
enum Parts {
	OP_LUI = 0b0110111,

	OP_AUIPC = 0b0010111,

	OP_JAL = 0b1101111,

	OP_JALR = 0b1100111,
	F3_JALR = 0b000,

	OP_LB = 0b0000011,
	F3_LB = 0b000,
	F3_LH = 0b001,
	F3_LW = 0b010,
	F3_LBU = 0b100,
	F3_LHU = 0b101,
	F3_LWU = 0b110,
	F3_LD = 0b011,

	OP_SB = 0b0100011,
	F3_SB = 0b000,
	F3_SH = 0b001,
	F3_SW = 0b010,
	F3_SD = 0b011,

	OP_BEQ = 0b1100011,
	F3_BEQ = 0b000,
	F3_BNE = 0b001,
	F3_BLT = 0b100,
	F3_BGE = 0b101,
	F3_BLTU = 0b110,
	F3_BGEU = 0b111,

	OP_ADDI = 0b0010011,
	F3_ADDI = 0b000,
	F3_SLTI = 0b010,
	F3_SLTIU = 0b011,
	F3_XORI = 0b100,
	F3_ORI = 0b110,
	F3_ANDI = 0b111,
	F3_SLLI = 0b001,
	F3_SRLI = 0b101,
	F7_SRLI = 0b0000000,
	F7_SRAI = 0b0100000,
	F6_SRLI = 0b000000,  // RV64 special case
	F6_SRAI = 0b010000,  // RV64 special case

	OP_ADD = 0b0110011,
	F3_ADD = 0b000,
	F7_ADD = 0b0000000,
	F3_SUB = 0b000,
	F7_SUB = 0b0100000,
	F3_SLL = 0b001,
	F3_SLT = 0b010,
	F3_SLTU = 0b011,
	F3_XOR = 0b100,
	F3_SRL = 0b101,
	F3_SRA = 0b101,
	F3_OR = 0b110,
	F3_AND = 0b111,

	F3_MUL = 0b000,
	F7_MUL = 0b0000001,
	F3_MULH = 0b001,
	F3_MULHSU = 0b010,
	F3_MULHU = 0b011,
	F3_DIV = 0b100,
	F3_DIVU = 0b101,
	F3_REM = 0b110,
	F3_REMU = 0b111,

	OP_FENCE = 0b0001111,
	F3_FENCE = 0b000,
	F3_FENCE_I = 0b001,

	OP_ECALL = 0b1110011,
	F3_SYS = 0b000,
	F12_ECALL = 0b000000000000,
	F12_EBREAK = 0b000000000001,
	// begin:privileged-instructions
	F12_URET = 0b000000000010,
	F12_SRET = 0b000100000010,
	F12_MRET = 0b001100000010,
	F12_WFI = 0b000100000101,
	F7_SFENCE_VMA = 0b0001001,
	// end:privileged-instructions
	F3_CSRRW = 0b001,
	F3_CSRRS = 0b010,
	F3_CSRRC = 0b011,
	F3_CSRRWI = 0b101,
	F3_CSRRSI = 0b110,
	F3_CSRRCI = 0b111,

	OP_AMO = 0b0101111,
	F5_LR_W = 0b00010,
	F5_SC_W = 0b00011,
	F5_AMOSWAP_W = 0b00001,
	F5_AMOADD_W = 0b00000,
	F5_AMOXOR_W = 0b00100,
	F5_AMOAND_W = 0b01100,
	F5_AMOOR_W = 0b01000,
	F5_AMOMIN_W = 0b10000,
	F5_AMOMAX_W = 0b10100,
	F5_AMOMINU_W = 0b11000,
	F5_AMOMAXU_W = 0b11100,

	F3_AMO_W = 0b010,
	F3_AMO_D = 0b011,

	OP_ADDIW = 0b0011011,
	F3_ADDIW = 0b000,
	F3_SLLIW = 0b001,
	F3_SRLIW = 0b101,
	F7_SRLIW = 0b0000000,
	F7_SRAIW = 0b0100000,

	OP_ADDW = 0b0111011,
	F3_ADDW = 0b000,
	F7_ADDW = 0b0000000,
	F3_SUBW = 0b000,
	F7_SUBW = 0b0100000,
	F3_SLLW = 0b001,
	F3_SRLW = 0b101,
	F7_SRLW = 0b0000000,
	F3_SRAW = 0b101,
	F7_SRAW = 0b0100000,
	F7_MULW = 0b0000001,
	F3_MULW = 0b000,
	F3_DIVW = 0b100,
	F3_DIVUW = 0b101,
	F3_REMW = 0b110,
	F3_REMUW = 0b111,

	// F and D extension
	OP_FMADD_S = 0b1000011,
	F2_FMADD_S = 0b00,
	F2_FMADD_D = 0b01,
	OP_FADD_S = 0b1010011,
	F7_FADD_S = 0b0000000,
	F7_FADD_D = 0b0000001,
	F7_FSUB_S = 0b0000100,
	F7_FSUB_D = 0b0000101,
	F7_FCVT_D_S = 0b0100001,
	F7_FMUL_S = 0b0001000,
	F7_FMUL_D = 0b0001001,
	F7_FDIV_S = 0b0001100,
	F7_FDIV_D = 0b0001101,
	F7_FLE_S = 0b1010000,
	F3_FLE_S = 0b000,
	F3_FLT_S = 0b001,
	F3_FEQ_S = 0b010,
	F7_FSGNJ_D = 0b0010001,
	F3_FSGNJ_D = 0b000,
	F3_FSGNJN_D = 0b001,
	F3_FSGNJX_D = 0b010,
	F7_FMIN_S = 0b0010100,
	F3_FMIN_S = 0b000,
	F3_FMAX_S = 0b001,
	F7_FMIN_D = 0b0010101,
	F3_FMIN_D = 0b000,
	F3_FMAX_D = 0b001,
	F7_FCVT_S_D = 0b0100000,
	F7_FSGNJ_S = 0b0010000,
	F3_FSGNJ_S = 0b000,
	F3_FSGNJN_S = 0b001,
	F3_FSGNJX_S = 0b010,
	F7_FLE_D = 0b1010001,
	F3_FLE_D = 0b000,
	F3_FLT_D = 0b001,
	F3_FEQ_D = 0b010,
	F7_FCVT_S_W = 0b1101000,
	RS2_FCVT_S_W = 0b00000,
	RS2_FCVT_S_WU = 0b00001,
	RS2_FCVT_S_L = 0b00010,
	RS2_FCVT_S_LU = 0b00011,
	F7_FCVT_D_W = 0b1101001,
	RS2_FCVT_D_W = 0b00000,
	RS2_FCVT_D_WU = 0b00001,
	RS2_FCVT_D_L = 0b00010,
	RS2_FCVT_D_LU = 0b00011,
	F7_FCVT_W_D = 0b1100001,
	RS2_FCVT_W_D = 0b00000,
	RS2_FCVT_WU_D = 0b00001,
	RS2_FCVT_L_D = 0b00010,
	RS2_FCVT_LU_D = 0b00011,
	F7_FSQRT_S = 0b0101100,
	F7_FSQRT_D = 0b0101101,
	F7_FCVT_W_S = 0b1100000,
	RS2_FCVT_W_S = 0b00000,
	RS2_FCVT_WU_S = 0b00001,
	RS2_FCVT_L_S = 0b00010,
	RS2_FCVT_LU_S = 0b00011,
	F7_FMV_X_W = 0b1110000,
	F3_FMV_X_W = 0b000,
	F3_FCLASS_S = 0b001,
	F7_FMV_X_D = 0b1110001,
	F3_FMV_X_D = 0b000,
	F3_FCLASS_D = 0b001,
	F7_FMV_W_X = 0b1111000,
	F7_FMV_D_X = 0b1111001,
	OP_FLW = 0b0000111,
	F3_FLW = 0b010,
	F3_FLD = 0b011,
	OP_FSW = 0b0100111,
	F3_FSW = 0b010,
	F3_FSD = 0b011,
	OP_FMSUB_S = 0b1000111,
	F2_FMSUB_S = 0b00,
	F2_FMSUB_D = 0b01,
	OP_FNMSUB_S = 0b1001011,
	F2_FNMSUB_S = 0b00,
	F2_FNMSUB_D = 0b01,
	OP_FNMADD_S = 0b1001111,
	F2_FNMADD_S = 0b00,
	F2_FNMADD_D = 0b01,

	// reserved opcodes for custom instructions
	OP_CUST1 = 0b0101011,
	OP_CUST0 = 0b0001011,

/*spandan-----------------------------------------*/	
	//reserved opcodes for CHERI
	OP_CHERI = 0b1011011,
	F3_INSPECT = 0b000,
	F7_INSPECT = 0b1111111,
	F7_INVOKE = 0x7e,
	F7_SPECIALRW = 0x1,
	F7_LDC = 0x7d,
	F7_SDC = 0x7c,

	RS2_CGETPERM = 0x0,
	RS2_CGETTYPE = 0x1,
	RS2_CGETBASE = 0x2,
	RS2_CGETLEN = 0x3,
	RS2_CGETTAG = 0x4,
	RS2_CGETSEALED = 0x5,
	RS2_CGETOFFSET = 0x6,
	RS2_CGETFLAGS = 0x7,
	RS2_CGETHIGH = 0x17,
	RS2_CGETTOP = 0x18,
	RS2_CGETADDR = 0xf, //deprecated
	RS2_CLEARTAG = 0xb,
	RS2_SEALENTRY = 0x11,
	RS2_MOVE = 0xa,
	RS2_JALRCAP = 0xc,
	RS2_JALRPCC = 0x14,
	RS2_CLEAR = 0xe,
	RS2_RRL = 0x8,
	RS2_RAM = 0x9,
	RS2_LOADTAGS = 0x12,
	RD_INVOKE = 0x1,

	RS2_LBC = 0x08,
	RS2_LHC = 0x09,
	RS2_LWC = 0x0a,
	RS2_LBUC = 0x0c,
	RS2_LHUC = 0x0d,
	RS2_LBDDC = 0x00,
	RS2_LHDDC = 0x01,
	RS2_LWDDC = 0x02,
	RS2_LBUDDC = 0x04,
	RS2_LHUDDC = 0x05,
	RS2_LCCAP = 0x0b,
	RS2_LCDDC = 0x03,

	RD_SBC = 0x08,
	RD_SHC = 0x09,
	RD_SWC = 0x0a,
	RD_SBDDC = 0x00,
	RD_SHDDC = 0x01,
	RD_SWDDC = 0x02,
	RD_SCCAP = 0x0b,
	RD_SCDDC = 0x03,

	F7_SEAL = 0xb,
	F7_UNSEAL = 0xc,
	F7_ANDPERM = 0xd,
	F7_SETFLAGS = 0xe,
	F7_SETOFFSET = 0xf,
	F7_SETADDR = 0x10,
	F7_INCOFFSET = 0x11,
	F3_INCOFFSETIMM = 0x1,
	F7_SETBOUNDS = 0x8,
	F7_SETBOUNDSEXACT = 0x9,
	F3_SETBOUNDSIMM = 0x2,
	F7_SETHIGH = 0x16,
	F7_BUILDCAP = 0x1d,
	F7_COPYTYPE = 0x1e,
	F7_CSEAL = 0x1f,
	F7_TESTSUBSET = 0x20,
	F7_SETEQUALEXACT = 0x21,
	F7_CFROMPTR = 0x13, //deprecated
	F7_CTOPTR = 0x12, //deprecated
	F7_CSUB = 0x14, //deprecated
/*spandan-----------------------------------------*/
};

// each instruction is mapped by the decoder to the following mapping
enum Mapping {
	UNDEF = 0,

	// RV32I base instruction set
	LUI = 1,
	AUIPC,
	JAL,
	JALR,
	BEQ,
	BNE,
	BLT,
	BGE,
	BLTU,
	BGEU,
	LB,
	LH,
	LW,
	LBU,
	LHU,
	SB,
	SH,
	SW,
	ADDI,
	SLTI,
	SLTIU,
	XORI,
	ORI,
	ANDI,
	SLLI,
	SRLI,
	SRAI,
	ADD,
	SUB,
	SLL,
	SLT,
	SLTU,
	XOR,
	SRL,
	SRA,
	OR,
	AND,
	FENCE,
	ECALL,
	EBREAK,

	// Zifencei standard extension
	FENCE_I,

	// Zicsr standard extension
	CSRRW,
	CSRRS,
	CSRRC,
	CSRRWI,
	CSRRSI,
	CSRRCI,

	// RV32M standard extension
	MUL,
	MULH,
	MULHSU,
	MULHU,
	DIV,
	DIVU,
	REM,
	REMU,

	// RV32A standard extension
	LR_W,
	SC_W,
	AMOSWAP_W,
	AMOADD_W,
	AMOXOR_W,
	AMOAND_W,
	AMOOR_W,
	AMOMIN_W,
	AMOMAX_W,
	AMOMINU_W,
	AMOMAXU_W,

	// RV64I base integer set (addition to RV32I)
	LWU,
	LD,
	SD,
	ADDIW,
	SLLIW,
	SRLIW,
	SRAIW,
	ADDW,
	SUBW,
	SLLW,
	SRLW,
	SRAW,

	// RV64M standard extension (addition to RV32M)
	MULW,
	DIVW,
	DIVUW,
	REMW,
	REMUW,

	// RV64A standard extension (addition to RV32A)
	LR_D,
	SC_D,
	AMOSWAP_D,
	AMOADD_D,
	AMOXOR_D,
	AMOAND_D,
	AMOOR_D,
	AMOMIN_D,
	AMOMAX_D,
	AMOMINU_D,
	AMOMAXU_D,

	// RV32F standard extension
	FLW,
	FSW,
	FMADD_S,
	FMSUB_S,
	FNMADD_S,
	FNMSUB_S,
	FADD_S,
	FSUB_S,
	FMUL_S,
	FDIV_S,
	FSQRT_S,
	FSGNJ_S,
	FSGNJN_S,
	FSGNJX_S,
	FMIN_S,
	FMAX_S,
	FCVT_W_S,
	FCVT_WU_S,
	FMV_X_W,
	FEQ_S,
	FLT_S,
	FLE_S,
	FCLASS_S,
	FCVT_S_W,
	FCVT_S_WU,
	FMV_W_X,

	// RV64F standard extension (addition to RV32F)
	FCVT_L_S,
	FCVT_LU_S,
	FCVT_S_L,
	FCVT_S_LU,

	// RV32D standard extension
	FLD,
	FSD,
	FMADD_D,
	FMSUB_D,
	FNMSUB_D,
	FNMADD_D,
	FADD_D,
	FSUB_D,
	FMUL_D,
	FDIV_D,
	FSQRT_D,
	FSGNJ_D,
	FSGNJN_D,
	FSGNJX_D,
	FMIN_D,
	FMAX_D,
	FCVT_S_D,
	FCVT_D_S,
	FEQ_D,
	FLT_D,
	FLE_D,
	FCLASS_D,
	FCVT_W_D,
	FCVT_WU_D,
	FCVT_D_W,
	FCVT_D_WU,

	// RV64D standard extension (addition to RV32D)
	FCVT_L_D,
	FCVT_LU_D,
	FMV_X_D,
	FCVT_D_L,
	FCVT_D_LU,
	FMV_D_X,

	// privileged instructions
	URET,
	SRET,
	MRET,
	WFI,
	SFENCE_VMA,

/*spandan-----------------------------*/
	//CHERI instructions
	CGETPERM,
	CGETTYPE,
	CGETBASE,
	CGETLEN,
	CGETTAG,
	CGETSEALED,
	CGETOFFSET,
	CGETFLAGS,
	CGETHIGH,
	CGETTOP,
	CSEAL,
	CUNSEAL,
	CANDPERM,
	CSETFLAGS,
	CSETOFFSET,
	CSETADDR,
	CINCOFFSET,
	CINCOFFSETIMM,
	CSETBOUNDS,
	CSETBOUNDSEXACT,
	CSETBOUNDSIMM,
	CSETHIGH,
	CCLEARTAG,
	CBUILDCAP,
	CCOPYTYPE,
	CCSEAL,
	CSEALENTRY,
	CMOVE,
	CTESTSUBSET,
	CSETEQUALEXACT,
	JALRCAP,
	JALRPCC,
	CINVOKE,
	CSPECIALRW,
	CCLEAR,
	CROUNDREPLEN,
	CREPALIGNMASK,
	CLOADTAGS,
	LBCAP,
	LHCAP,
	LWCAP,
	LBUCAP,
	LHUCAP,
	LBDDC,
	LHDDC,
	LWDDC,
	LBUDDC,
	LHUDDC,
	LCCAP,
	LCDDC,
	SBCAP,
	SHCAP,
	SWCAP,
	SBDDC,
	SHDDC,
	SWDDC,
	SCCAP,
	SCDDC,
//Deprecated instructions
	CGETADDR,
	CFROMPTR,
	CTOPTR,
	CSUB,
/*spandan-----------------------------*/

	NUMBER_OF_INSTRUCTIONS
};

// type denotes the instruction format
enum class Type {
	UNKNOWN = 0,
	R,
	I,
	S,
	B,
	U,
	J,
	R4,
};

extern std::array<const char*, NUMBER_OF_INSTRUCTIONS> mappingStr;
extern std::array<const char*, 32> regnamePrettyStr;

Type getType(Mapping mapping);
}  // namespace Opcode

/*spandan--------------------------------------*/
#define TOP_TWO(top) ((top >> 31) & 0x3)
/*spandan--------------------------------------*/
#define BIT_RANGE(instr, upper, lower) (instr & (((1 << (upper - lower + 1)) - 1) << lower))
#define BIT_SLICE(instr, upper, lower) (BIT_RANGE(instr, upper, lower) >> lower)
#define BIT_SINGLE(instr, pos) (instr & (1 << pos))
#define BIT_SINGLE_P1(instr, pos) (BIT_SINGLE(instr, pos) >> pos)
#define BIT_SINGLE_PN(instr, pos, new_pos) ((BIT_SINGLE(instr, pos) >> pos) << new_pos)
#define EXTRACT_SIGN_BIT(instr, pos, new_pos) ((BIT_SINGLE_P1(instr, pos) << 31) >> (31 - new_pos))
/*spandan--------------------------------------------------------*/
//By bitwise and, replaces start to end bits by 0
#define REPLACEMENT_MASK(start, end) (~(((0b1 << (end - start + 1)) - 1) << start))
//Set or unset pos bit of instr
#define SET_BIT(instr, pos) (instr | (1 << pos))
#define UNSET_BIT(instr, pos) (instr & (~(1 << pos)))
/*spandan--------------------------------------------------------*/

struct Instruction {
	Instruction() : instr(0) {}

	Instruction(uint32_t instr) : instr(instr) {}

	inline uint32_t quadrant() {
		return instr & 0x3;
	}

	inline bool is_compressed() {
		return quadrant() < 3;
	}

	inline uint32_t c_format() {
		return instr & 0xffff;
	}

	inline uint32_t c_opcode() {
		return BIT_SLICE(instr, 15, 13);
	}

	inline uint32_t c_b12() {
		return BIT_SINGLE_P1(instr, 12);
	}

	inline uint32_t c_rd() {
		return rd();
	}

	inline uint32_t c_rd_small() {
		return BIT_SLICE(instr, 9, 7) | 8;
	}

	inline uint32_t c_rs2_small() {
		return BIT_SLICE(instr, 4, 2) | 8;
	}

	inline uint32_t c_rs2() {
		return BIT_SLICE(instr, 6, 2);
	}

	inline uint32_t c_imm() {
		return BIT_SLICE(instr, 6, 2) | EXTRACT_SIGN_BIT(instr, 12, 5);
	}

	inline uint32_t c_uimm() {
		return BIT_SLICE(instr, 6, 2) | (BIT_SINGLE_P1(instr, 12) << 5);
	}

	inline uint32_t c_f2_high() {
		return BIT_SLICE(instr, 11, 10);
	}

	inline uint32_t c_f2_low() {
		return BIT_SLICE(instr, 6, 5);
	}

	Opcode::Mapping decode_normal(Architecture arch);

	Opcode::Mapping decode_and_expand_compressed(Architecture arch);

	inline uint32_t csr() {
		// cast to unsigned to avoid sign extension when shifting
		return BIT_RANGE((uint32_t)instr, 31, 20) >> 20;
	}

	inline uint32_t zimm() {
		return BIT_RANGE(instr, 19, 15) >> 15;
	}

	inline unsigned shamt() {
		return (BIT_RANGE(instr, 25, 20) >> 20);
	}

	inline unsigned shamt_w() {
		return (BIT_RANGE(instr, 24, 20) >> 20);
	}

	inline int32_t funct2() {
		return (BIT_RANGE(instr, 26, 25) >> 25);
	}

	inline int32_t funct3() {
		return (BIT_RANGE(instr, 14, 12) >> 12);
	}

/*spandan-----------------------------------------------*/
	inline int32_t inspect2() {
		return (BIT_RANGE(instr, 24, 20) >> 20);
	}

	inline int32_t inspectd() {
		return (BIT_RANGE(instr, 11, 7) >> 7);
	}

	inline uint8_t q() {
		return (BIT_SLICE(instr, 19, 18));
	}

	inline uint8_t m_upper() {
		return (BIT_SLICE(instr, 17, 15));
	}

	inline uint8_t m_lower() {
		return (BIT_SLICE(instr, 11, 7));
	}
/*spandan-----------------------------------------------*/

	inline int32_t funct12() {
		// cast to unsigned to avoid sign extension when shifting
		return (BIT_RANGE((uint32_t)instr, 31, 20) >> 20);
	}

	inline int32_t funct7() {
		// cast to unsigned to avoid sign extension when shifting
		return (BIT_RANGE((uint32_t)instr, 31, 25) >> 25);
	}

	inline int32_t funct6() {
		// cast to unsigned to avoid sign extension when shifting
		return (BIT_RANGE((uint32_t)instr, 31, 26) >> 26);
	}

	inline int32_t funct5() {
		// cast to unsigned to avoid sign extension when shifting
		return (BIT_RANGE((uint32_t)instr, 31, 27) >> 27);
	}

	inline uint32_t frm() {
		return BIT_SLICE(instr, 14, 12);
	}

	inline uint32_t fence_succ() {
		return BIT_SLICE(instr, 23, 20);
	}

	inline uint32_t fence_pred() {
		return BIT_SLICE(instr, 27, 24);
	}

	inline uint32_t fence_fm() {
		return BIT_SLICE(instr, 31, 28);
	}

	inline bool aq() {
		return BIT_SINGLE(instr, 26);
	}

	inline bool rl() {
		return BIT_SINGLE(instr, 25);
	}

	inline int32_t opcode() {
		return BIT_RANGE(instr, 6, 0);
	}

	inline int32_t J_imm() {
		return (BIT_SINGLE(instr, 31) >> 11) | BIT_RANGE(instr, 19, 12) | (BIT_SINGLE(instr, 20) >> 9) |
		       (BIT_RANGE(instr, 30, 21) >> 20);
	}

	inline int32_t I_imm() {
		return BIT_RANGE(instr, 31, 20) >> 20;
	}

/*spandan--------------------------------------------*/
//Required for CSETBOUNDSIMM
	inline uint32_t unsigned_I_imm() {
		return ((static_cast<uint32_t>(instr) >> 20) & 0xFFF);
	}
/*spandan--------------------------------------------*/

	inline int32_t S_imm() {
		return (BIT_RANGE(instr, 31, 25) >> 20) | (BIT_RANGE(instr, 11, 7) >> 7);
	}

	inline int32_t B_imm() {
		return (BIT_SINGLE(instr, 31) >> 19) | (BIT_SINGLE(instr, 7) << 4) | (BIT_RANGE(instr, 30, 25) >> 20) |
		       (BIT_RANGE(instr, 11, 8) >> 7);
	}

	inline int32_t U_imm() {
		return BIT_RANGE(instr, 31, 12);
	}

	inline uint32_t rs1() {
		return BIT_RANGE(instr, 19, 15) >> 15;
	}

	inline uint32_t rs2() {
		return BIT_RANGE(instr, 24, 20) >> 20;
	}

	inline uint32_t rs3() {
		return BIT_RANGE((uint32_t)instr, 31, 27) >> 27;
	}

	inline uint32_t rd() {
		return BIT_RANGE(instr, 11, 7) >> 7;
	}

	inline uint32_t data() {
		return instr;
	}

   private:
	// use signed variable to have correct sign extension in immediates
	int32_t instr;
};

#endif  // RISCV_ISA_INSTR_H
