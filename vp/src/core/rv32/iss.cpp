#include "iss.h"

// to save *cout* format setting, see *ISS::show*
#include <boost/io/ios_state.hpp>
// for safe down-cast
#include <boost/lexical_cast.hpp>

using namespace rv32;

#define RAISE_ILLEGAL_INSTRUCTION() raise_trap(EXC_ILLEGAL_INSTR, instr.data());

#define REQUIRE_ISA(X)          \
    if (!(csrs.misa.reg & X))   \
        RAISE_ILLEGAL_INSTRUCTION()

#define RD instr.rd()
#define RS1 instr.rs1()
#define RS2 instr.rs2()
#define RS3 instr.rs3()


const char *regnames[] = {
    "zero (x0)", "ra   (x1)", "sp   (x2)", "gp   (x3)", "tp   (x4)", "t0   (x5)", "t1   (x6)", "t2   (x7)",
    "s0/fp(x8)", "s1   (x9)", "a0  (x10)", "a1  (x11)", "a2  (x12)", "a3  (x13)", "a4  (x14)", "a5  (x15)",
    "a6  (x16)", "a7  (x17)", "s2  (x18)", "s3  (x19)", "s4  (x20)", "s5  (x21)", "s6  (x22)", "s7  (x23)",
    "s8  (x24)", "s9  (x25)", "s10 (x26)", "s11 (x27)", "t3  (x28)", "t4  (x29)", "t5  (x30)", "t6  (x31)",
};

/*spandan------------------------------------------------------------------------------------------------------*/
const char *cherinames[] = {
    "czero (x0)", "cra   (x1)", "csp   (x2)", "cgp   (x3)", "ctp   (x4)", "ct0   (x5)", "ct1   (x6)", "ct2   (x7)",
    "cs0/fp(x8)", "cs1   (x9)", "ca0  (x10)", "ca1  (x11)", "ca2  (x12)", "ca3  (x13)", "ca4  (x14)", "ca5  (x15)",
    "ca6  (x16)", "ca7  (x17)", "cs2  (x18)", "cs3  (x19)", "cs4  (x20)", "cs5  (x21)", "cs6  (x22)", "cs7  (x23)",
    "cs8  (x24)", "cs9  (x25)", "cs10 (x26)", "cs11 (x27)", "ct3  (x28)", "ct4  (x29)", "ct5  (x30)", "ct6  (x31)",
};

const char *tagname = "tag";
/*spandan------------------------------------------------------------------------------------------------------*/

int regcolors[] = {
#if defined(COLOR_THEME_DARK)
    0,  1,  2,  3,  4,  5,  6,  52, 8,  9,  53, 54, 55, 56, 57, 58,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
#elif defined(COLOR_THEME_LIGHT)
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 153, 154, 155, 156, 157, 158,
    116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
#else
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
};

RegFile::RegFile() {
	memset(regs, 0, sizeof(regs));

	/*spandan-----------------------------*/
	for (size_t i = 0; i < NUM_REGS; ++i) {
    	cheri[i] = NULL_CAP;
	}
	/*spandan-----------------------------*/

	memset(&tags, 0, sizeof(uint32_t)); //spandan
}

RegFile::RegFile(const RegFile &other) {
	memcpy(regs, other.regs, sizeof(regs));
}

void RegFile::write(uint32_t index, int32_t value) {
	assert(index <= x31);
	assert(index != x0);
	regs[index] = value;
}

int32_t RegFile::read(uint32_t index) {
	if (index > x31)
		throw std::out_of_range("out-of-range register access");
	return regs[index];
}

uint32_t RegFile::shamt(uint32_t index) {
	assert(index <= x31);
	return BIT_RANGE(regs[index], 4, 0);
}

int32_t &RegFile::operator[](const uint32_t idx) {
	return regs[idx];
}

#if defined(COLOR_THEME_LIGHT) || defined(COLOR_THEME_DARK)
#define COLORFRMT "\e[38;5;%um%s\e[39m"
#define COLORPRINT(fmt, data) fmt, data
#else
#define COLORFRMT "%s"
#define COLORPRINT(fmt, data) data
#endif

void RegFile::show() {
/*spandan-----------------------------------------*/
	for (unsigned i = 0; i < NUM_REGS; ++i) {
		printf(COLORFRMT " = %8x\t" COLORFRMT " = %8x\t" COLORFRMT " = %x\n", \
			COLORPRINT(regcolors[i], regnames[i]), regs[i], \
			COLORPRINT(regcolors[i], cherinames[i]), cheri[i], \
			COLORPRINT(regcolors[i], tagname), BIT_SINGLE_P1(tags, i));
	}
/*spandan-----------------------------------------*/
}

ISS::ISS(uint32_t hart_id, bool use_E_base_isa) : systemc_name("Core-" + std::to_string(hart_id)) {
	csrs.mhartid.reg = hart_id;
	if (use_E_base_isa)
		csrs.misa.select_E_base_isa();

	sc_core::sc_time qt = tlm::tlm_global_quantum::instance().get();
	cycle_time = sc_core::sc_time(10, sc_core::SC_NS);

	assert(qt >= cycle_time);
	assert(qt % cycle_time == sc_core::SC_ZERO_TIME);

	for (int i = 0; i < Opcode::NUMBER_OF_INSTRUCTIONS; ++i) instr_cycles[i] = cycle_time;

	const sc_core::sc_time memory_access_cycles = 4 * cycle_time;
	const sc_core::sc_time mul_div_cycles = 8 * cycle_time;

/*spandan-------------------------------------------*/
	/*
	instr_cycles[Opcode::LBCAP] = memory_access_cycles;
	instr_cycles[Opcode::LHCAP] = memory_access_cycles;
	instr_cycles[Opcode::LWCAP] = memory_access_cycles;
	instr_cycles[Opcode::LBUCAP] = memory_access_cycles;
	instr_cycles[Opcode::LHUCAP] = memory_access_cycles;
	instr_cycles[Opcode::LBDDC] = memory_access_cycles;
	instr_cycles[Opcode::LHDDC] = memory_access_cycles;
	instr_cycles[Opcode::LWDDC] = memory_access_cycles;
	instr_cycles[Opcode::LBUDDC] = memory_access_cycles;
	instr_cycles[Opcode::LHUDDC] = memory_access_cycles;
	instr_cycles[Opcode::SBCAP] = memory_access_cycles;
	instr_cycles[Opcode::SHCAP] = memory_access_cycles;
	instr_cycles[Opcode::SWCAP] = memory_access_cycles;
	instr_cycles[Opcode::SBDDC] = memory_access_cycles;
	instr_cycles[Opcode::SHDDC] = memory_access_cycles;
	instr_cycles[Opcode::SWDDC] = memory_access_cycles;
	*/
/*spandan-------------------------------------------*/
	instr_cycles[Opcode::LB] = memory_access_cycles;
	instr_cycles[Opcode::LBU] = memory_access_cycles;
	instr_cycles[Opcode::LH] = memory_access_cycles;
	instr_cycles[Opcode::LHU] = memory_access_cycles;
	instr_cycles[Opcode::LW] = memory_access_cycles;
	instr_cycles[Opcode::SB] = memory_access_cycles;
	instr_cycles[Opcode::SH] = memory_access_cycles;
	instr_cycles[Opcode::SW] = memory_access_cycles;
	instr_cycles[Opcode::MUL] = mul_div_cycles;
	instr_cycles[Opcode::MULH] = mul_div_cycles;
	instr_cycles[Opcode::MULHU] = mul_div_cycles;
	instr_cycles[Opcode::MULHSU] = mul_div_cycles;
	instr_cycles[Opcode::DIV] = mul_div_cycles;
	instr_cycles[Opcode::DIVU] = mul_div_cycles;
	instr_cycles[Opcode::REM] = mul_div_cycles;
	instr_cycles[Opcode::REMU] = mul_div_cycles;
	op = Opcode::UNDEF;
}

void ISS::exec_step() {
	/*spandan-----------------------*/
	Capability cs;
	cs.addr = pc;
	cs.caps = cpc;
	cs.tag = pc_tag;

#ifdef TESTRIG
	// PC for currect instruction
	rvfi_dii_output.rvfi_dii_pc_rdata = static_cast<uint64_t>(pc);
#endif
	check_fetch_exceptions(32, cs);
	/*spandan-----------------------*/

#ifdef TESTRIG
	// In TestRIG mode, always raise an exception
	// Raise exception for misaligned instruction
	if ((pc & ~pc_alignment_mask()) != 0) {
		std::cout << "[assert] Warn: misaligned instruction" << std::endl;
		raise_trap(EXC_INSTR_ADDR_MISALIGNED, pc);
	}
#else
	assert(((pc & ~pc_alignment_mask()) == 0) && "misaligned instruction");
#endif

	try {
#ifdef TESTRIG
		instr = Instruction(rvfi_dii_input.rvfi_dii_insn);
		rvfi_dii_output.rvfi_dii_insn = rvfi_dii_input.rvfi_dii_insn;
#else
		// In non-TESTRIG mode, fetch instruction from memory
		uint32_t mem_word = instr_mem->load_instr(pc);
		instr = Instruction(mem_word);
#endif
	} catch (SimulationTrap &e) {
	/*spandan--------------------------*/
		if (trace)
			std::cout << "take trap " << e.reason << ", mtval=" << e.mtval << std::endl;
		auto target_mode = prepare_trap(e);
		switch_to_trap_handler(target_mode);
	/*spandan--------------------------*/
		/*
		op = Opcode::UNDEF;
		instr = Instruction(0);
		throw;
		*/
	}

	// Sayak_pc_increment
	if (instr.is_compressed()) {
		op = instr.decode_and_expand_compressed(RV32);
		pc += 2;
        if (op != Opcode::UNDEF)
            REQUIRE_ISA(C_ISA_EXT);
    } else {
		op = instr.decode_normal(RV32);
		pc += 4;
#ifdef TESTRIG
		rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif
	}

	if (trace) {
		printf("core %2u: prv %1x: pc %8x: %s ", csrs.mhartid.reg, prv, last_pc, Opcode::mappingStr[op]);
		switch (Opcode::getType(op)) {
			case Opcode::Type::R:
				printf(COLORFRMT ", " COLORFRMT ", " COLORFRMT, COLORPRINT(regcolors[instr.rd()], regnames[instr.rd()]),
				       COLORPRINT(regcolors[instr.rs1()], regnames[instr.rs1()]),
				       COLORPRINT(regcolors[instr.rs2()], regnames[instr.rs2()]));
				break;
			case Opcode::Type::I:
				printf(COLORFRMT ", " COLORFRMT ", 0x%x", COLORPRINT(regcolors[instr.rd()], regnames[instr.rd()]),
				       COLORPRINT(regcolors[instr.rs1()], regnames[instr.rs1()]), instr.I_imm());
				break;
			case Opcode::Type::S:
				printf(COLORFRMT ", " COLORFRMT ", 0x%x", COLORPRINT(regcolors[instr.rs1()], regnames[instr.rs1()]),
				       COLORPRINT(regcolors[instr.rs2()], regnames[instr.rs2()]), instr.S_imm());
				break;
			case Opcode::Type::B:
				printf(COLORFRMT ", " COLORFRMT ", 0x%x", COLORPRINT(regcolors[instr.rs1()], regnames[instr.rs1()]),
				       COLORPRINT(regcolors[instr.rs2()], regnames[instr.rs2()]), instr.B_imm());
				break;
			case Opcode::Type::U:
				printf(COLORFRMT ", 0x%x", COLORPRINT(regcolors[instr.rd()], regnames[instr.rd()]), instr.U_imm());
				break;
			case Opcode::Type::J:
				printf(COLORFRMT ", 0x%x", COLORPRINT(regcolors[instr.rd()], regnames[instr.rd()]), instr.J_imm());
				break;
			default:;
		}
		puts("");
	}

	switch (op) {
		case Opcode::UNDEF:
			if (trace)
				std::cout << "[ISS] WARNING: unknown instruction '" << std::to_string(instr.data()) << "' at address '"
				          << std::to_string(last_pc) << "'" << std::endl;
			raise_trap(EXC_ILLEGAL_INSTR, instr.data());
			break;

		case Opcode::ADDI: {
			regs[instr.rd()] = regs[instr.rs1()] + instr.I_imm();
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif
			}
			break;

		case Opcode::SLTI: {
			regs[instr.rd()] = regs[instr.rs1()] < instr.I_imm();
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif
			}
			break;

		case Opcode::SLTIU: {
			regs[instr.rd()] = ((uint32_t)regs[instr.rs1()]) < ((uint32_t)instr.I_imm());
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif
			}
			break;

		case Opcode::XORI: {
			regs[instr.rd()] = regs[instr.rs1()] ^ instr.I_imm();
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif
			}
			break;

		case Opcode::ORI: {
			regs[instr.rd()] = regs[instr.rs1()] | instr.I_imm();
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif
			}
			break;

		case Opcode::ANDI: {
			regs[instr.rd()] = regs[instr.rs1()] & instr.I_imm();
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif
			}
			break;

		case Opcode::ADD: {
			regs[instr.rd()] = regs[instr.rs1()] + regs[instr.rs2()];
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
			}
			break;

		case Opcode::SUB: {
			regs[instr.rd()] = regs[instr.rs1()] - regs[instr.rs2()];
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
			}
			break;

		case Opcode::SLL: {
			regs[instr.rd()] = regs[instr.rs1()] << regs.shamt(instr.rs2());
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instruction, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
			}
			break;

		case Opcode::SLT: {
			regs[instr.rd()] = regs[instr.rs1()] < regs[instr.rs2()];
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instruction, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
			}
			break;

		case Opcode::SLTU: {
			regs[instr.rd()] = ((uint32_t)regs[instr.rs1()]) < ((uint32_t)regs[instr.rs2()]);
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instruction, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
			}
			break;

		case Opcode::SRL: {
			regs[instr.rd()] = ((uint32_t)regs[instr.rs1()]) >> regs.shamt(instr.rs2());
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instruction, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
			}
			break;

		case Opcode::SRA: {
			regs[instr.rd()] = regs[instr.rs1()] >> regs.shamt(instr.rs2());
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instruction, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
			}
			break;

		case Opcode::XOR: {
			regs[instr.rd()] = regs[instr.rs1()] ^ regs[instr.rs2()];
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instruction, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
			}
			break;

		case Opcode::OR: {
			regs[instr.rd()] = regs[instr.rs1()] | regs[instr.rs2()];
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instruction, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
			}
			break;

		case Opcode::AND: {
			regs[instr.rd()] = regs[instr.rs1()] & regs[instr.rs2()];
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instruction, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
			}
			break;

		case Opcode::SLLI: {
			regs[instr.rd()] = regs[instr.rs1()] << instr.shamt();
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instruction, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif
			}
			break;

		case Opcode::SRLI: {
			regs[instr.rd()] = ((uint32_t)regs[instr.rs1()]) >> instr.shamt();
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instruction, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif
			}
			break;

		case Opcode::SRAI: {
			regs[instr.rd()] = regs[instr.rs1()] >> instr.shamt();
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instruction, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif
			}
			break;

		case Opcode::LUI: {
			regs[instr.rd()] = instr.U_imm();
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instruction, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
//		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif
			}
			break;


/*spandan: CHERI cap read + modify--------------------------------------------------------------------*/
		case Opcode::CGETPERM:
			REQUIRE_ISA(CHERI_ISA_EXT);
			regs[instr.rd()] = BIT_SLICE(regs.cheri[instr.rs1()], CAP_PERM_TOP, CAP_PERM_BOT) | 0;
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

			break;

		case Opcode::CGETTYPE: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			int32_t otype = BIT_SLICE(regs.cheri[instr.rs1()], CAP_OTYPE_TOP, CAP_OTYPE_BOT);
			if (otype <= Reserved) 
				regs[instr.rd()] = otype;
			else {
				// Sign extend the otype value to fit into the target register
				uint8_t shift = ADDR_BITS - (CAP_OTYPE_TOP - CAP_OTYPE_BOT + 1);
				regs[instr.rd()] = (int32_t)(otype << shift) >> shift;
			}
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

			}
			break;

		case Opcode::CGETBASE:
			REQUIRE_ISA(CHERI_ISA_EXT);
			regs[instr.rd()] = get_base(regs[instr.rs1()], regs.cheri[instr.rs1()]);
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

			break;

		case Opcode::CGETLEN:
			REQUIRE_ISA(CHERI_ISA_EXT);
			regs[instr.rd()] = get_len(regs[instr.rs1()], regs.cheri[instr.rs1()]);
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

			break;

		case Opcode::CGETTAG:
			REQUIRE_ISA(CHERI_ISA_EXT);
			regs[instr.rd()] = BIT_SINGLE_P1( (regs.tags), (instr.rs1()) );
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

			break;

		case Opcode::CGETSEALED: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			uint32_t otype = BIT_SLICE(regs.cheri[instr.rs1()], CAP_OTYPE_TOP, CAP_OTYPE_BOT);
			if (otype == Unsealed) regs[instr.rd()] = 0b0;
			else regs[instr.rd()] = 0b1;
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

			}
			break;

		case Opcode::CGETOFFSET:
			REQUIRE_ISA(CHERI_ISA_EXT);
			regs[instr.rd()] = static_cast<int64_t>(static_cast<uint32_t>(regs[instr.rs1()])) - \
							   static_cast<int64_t>(get_base(regs[instr.rs1()], regs.cheri[instr.rs1()]));
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

			break;

		case Opcode::CGETFLAGS:
			REQUIRE_ISA(CHERI_ISA_EXT);
			if (BIT_SINGLE( (regs.cheri[instr.rs1()]), CAP_FLAG))	
				regs[instr.rd()] = 0x1;
			else regs[instr.rd()] = 0x0;
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

			break;

		case Opcode::CGETHIGH:
			REQUIRE_ISA(CHERI_ISA_EXT);
			regs[instr.rd()] = (static_cast<uint32_t>(regs.cheri[instr.rs1()])) ^ ((NULL_CAP) & 0xFFFFFFFF);

			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

			break;

		case Opcode::CGETTOP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			int64_t t = get_top(regs[instr.rs1()], regs.cheri[instr.rs1()]);
			if (t > (int64_t)(MAX_CAP_ADDR))
				regs[instr.rd()] = MAX_CAP_ADDR;
			else 
				regs[instr.rd()] = t;
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

			}
			break;

#ifdef TESTRIG
// Deprecated instruction, implemented for TestRIG-------------------------------------------------------------
		case Opcode::CGETADDR:
			REQUIRE_ISA(CHERI_ISA_EXT);
			regs[instr.rd()] = static_cast<uint32_t>(regs[instr.rs1()]);
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

			break;
// Deprecated instruction, implemented for TestRIG--------------------------------------------------------------
#endif

		case Opcode::CSEAL: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1, cs2, neu;
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = BIT_SINGLE_P1( (regs.tags), (instr.rs1()) );
			cs2.addr = static_cast<uint32_t>(regs[instr.rs2()]);
			cs2.caps = regs.cheri[instr.rs2()];
			cs2.tag = BIT_SINGLE_P1( (regs.tags), (instr.rs2()) );
			neu = seal_cap(cs1, cs2);

			if (neu.tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

			regs[instr.rd()] = neu.addr;
			regs.cheri[instr.rd()] = neu.caps;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

			}
			break;


		case Opcode::CUNSEAL: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1, cs2, neu;
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = BIT_SINGLE_P1( (regs.tags), (instr.rs1()) );
			cs2.addr = static_cast<uint32_t>(regs[instr.rs2()]);
			cs2.caps = regs.cheri[instr.rs2()];
			cs2.tag = BIT_SINGLE_P1( (regs.tags), (instr.rs2()) );
			neu = unseal_cap(cs1, cs2);

			if (neu.tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

			regs[instr.rd()] = neu.addr;
			regs.cheri[instr.rd()] = neu.caps;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

			}
			break;


		case Opcode::CANDPERM: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			uint32_t cs1_caps = regs.cheri[instr.rs1()];
			uint32_t cs2_addr = static_cast<uint32_t>(regs[instr.rs2()]);

			uint32_t neu_perms = BIT_RANGE(cs2_addr, (CAP_PERM_TOP-CAP_PERM_BOT), 0);
			uint8_t cs1_otype = BIT_SLICE(cs1_caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT);

			if ((cs1_otype != Unsealed) || (BIT_SINGLE( (regs.tags), (instr.rs1()) ) == 0)) 
				regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = SET_BIT( (regs.tags), (instr.rd()) );

			regs.cheri[instr.rd()] = replace_bits(cs1_caps, CAP_PERM_BOT, CAP_PERM_TOP, neu_perms);
			regs[instr.rd()] = regs[instr.rs1()];

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		} break;

		case Opcode::CSETFLAGS: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			regs[instr.rd()] = regs[instr.rs1()];
			regs.cheri[instr.rd()] = regs.cheri[instr.rs1()];
			if (BIT_SINGLE( (regs.tags), (instr.rs1()) )) 
				regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
			else
				regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

			int8_t flag_value = BIT_SLICE( (regs[instr.rs2()]), (CAP_FLAGS_WIDTH-1), 0);
			regs.cheri[instr.rd()] = replace_bits( (regs.cheri[instr.rd()]), CAP_FLAG, CAP_FLAG, flag_value );

			uint8_t cs1_otype = BIT_SLICE((regs.cheri[instr.rs1()]), CAP_OTYPE_TOP, CAP_OTYPE_BOT);
			if ((cs1_otype != Unsealed) || (BIT_SINGLE( (regs.tags), (instr.rs1()) ) == 0)) 
				regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = SET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		} break;


		case Opcode::CSETOFFSET: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;
			bool fast_rep_check, neu_tag = 1;
			uint32_t rs2 = static_cast<uint32_t>(regs[instr.rs2()]);
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			uint8_t cs1_otype = BIT_SLICE((cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT); 
			
			if ((cs1_otype != Unsealed) || (BIT_SINGLE( (regs.tags), (instr.rs1()) ) == 0)) 
				neu_tag = 0;
			if (neu_tag) {
				fast_rep_check = set_offset(cs1, rs2, true);

				if (fast_rep_check) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
				else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			}
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			
			//Set address to base address
			cs1.addr = get_base(cs1.addr, cs1.caps);
			regs[instr.rd()] = static_cast<uint32_t>(static_cast<int64_t>(cs1.addr) + rs2);
			regs.cheri[instr.rd()] = cs1.caps;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		} 
		break;


		case Opcode::CSETADDR: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;
			bool fast_rep_check, neu_tag = 1;
			uint32_t rep_addr = static_cast<uint32_t>(regs[instr.rs2()]);
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			int64_t offset = static_cast<int64_t>(rep_addr) - static_cast<int64_t>(cs1.addr);
			cs1.caps = regs.cheri[instr.rs1()];
			uint8_t cs1_otype = BIT_SLICE((cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT); 
			
			if ((cs1_otype != Unsealed) || (BIT_SINGLE( (regs.tags), (instr.rs1()) ) == 0)) 
				neu_tag = 0;
			if (neu_tag) {
				fast_rep_check = set_offset(cs1, offset, false);

				if (fast_rep_check) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
				else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			}
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			
			regs[instr.rd()] = rep_addr;
			regs.cheri[instr.rd()] = cs1.caps;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		}
		break;


		case Opcode::CINCOFFSET: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;
			bool fast_rep_check, neu_tag = 1;
			uint32_t rs2 = static_cast<uint32_t>(regs[instr.rs2()]);
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			uint8_t cs1_otype = BIT_SLICE((cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT); 
			
			if ((cs1_otype != Unsealed) || (BIT_SINGLE( (regs.tags), (instr.rs1()) ) == 0)) 
				neu_tag = 0;
			if (neu_tag) {
				fast_rep_check = set_offset(cs1, rs2, false);

				if (fast_rep_check) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
				else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			}
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			
			regs[instr.rd()] = static_cast<uint32_t>(static_cast<int64_t>(cs1.addr) + rs2);
			regs.cheri[instr.rd()] = cs1.caps;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		}
		break;


		case Opcode::CINCOFFSETIMM: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;
			bool fast_rep_check, neu_tag = 1;
			int32_t rs2 = (instr.I_imm());
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			uint8_t cs1_otype = BIT_SLICE((cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT); 
			
			if ((cs1_otype != Unsealed) || (BIT_SINGLE( (regs.tags), (instr.rs1()) ) == 0)) 
				neu_tag = 0;
			if (neu_tag) {
				fast_rep_check = set_offset(cs1, rs2, false);

				if (fast_rep_check) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
				else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			}
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			
			regs[instr.rd()] = static_cast<uint32_t>(static_cast<int64_t>(cs1.addr) + rs2);
			regs.cheri[instr.rd()] = cs1.caps;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

		}
		break;


		case Opcode::CSETBOUNDS: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;
			SetBound_Ret final;
			SetBound_flag choice;
			bool neu_tag = 1, final_tag;
			uint32_t b;
			int64_t t;
			uint32_t rs2 = static_cast<uint32_t>(regs[instr.rs2()]);
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			uint8_t cs1_otype = BIT_SLICE((cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT); 
			
			if ((cs1_otype != Unsealed) || (BIT_SINGLE( (regs.tags), (instr.rs1()) ) == 0))
				neu_tag = 0;
			if (neu_tag) {
				//Check if requested base and top are valid
				b = get_base(cs1.addr, cs1.caps);
				t = get_top(cs1.addr, cs1.caps);
				final_tag = (((cs1.addr) >= b) && ((cs1.addr + rs2) <= t));
				
				if (final_tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
				else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			}
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			
			choice.len = false;
			choice.mask = false;
			final = set_bounds(cs1,rs2,choice);
			regs[instr.rd()] = final.cd.addr;
			regs.cheri[instr.rd()] = final.cd.caps;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		}
		break;


		case Opcode::CSETBOUNDSEXACT: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;
			SetBound_Ret final;
			SetBound_flag choice;
			bool neu_tag = 1, final_tag;
			uint32_t b;
			int64_t t;
			uint32_t rs2 = static_cast<uint32_t>(regs[instr.rs2()]);
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			uint8_t cs1_otype = BIT_SLICE((cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT); 
			
			if ((cs1_otype != Unsealed) || (BIT_SINGLE( (regs.tags), (instr.rs1()) ) == 0)) 
				neu_tag = 0;
			if (neu_tag) {
				//Check if requested base and top are valid
				b = get_base(cs1.addr, cs1.caps);
				t = get_top(cs1.addr, cs1.caps);
				final_tag = (((cs1.addr) >= b) && ((cs1.addr + rs2) <= t));

				if (final_tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
				else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			}
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			
			choice.len = false;
			choice.mask = false;
			final = set_bounds(cs1,rs2,choice);
			if (not(final.exact)) regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			regs[instr.rd()] = final.cd.addr;
			regs.cheri[instr.rd()] = final.cd.caps;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		}
		break;


		case Opcode::CSETBOUNDSIMM: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;
			SetBound_Ret final;
			SetBound_flag choice;
			bool neu_tag = 1, final_tag;
			uint32_t b;
			int64_t t;
			uint32_t rs2 = instr.unsigned_I_imm(); //The immediate is unsigned
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			uint8_t cs1_otype = BIT_SLICE((cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT); 
			
			if ((cs1_otype != Unsealed) || (BIT_SINGLE( (regs.tags), (instr.rs1()) ) == 0))
				neu_tag = 0;
			if (neu_tag) {
				//Check if requested base and top are valid
				b = get_base(cs1.addr, cs1.caps);
				t = get_top(cs1.addr, cs1.caps);
				final_tag = (((cs1.addr) >= b) && ((cs1.addr + rs2) <= t));

				if (final_tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
				else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			}
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			
			choice.len = false;
			choice.mask = false;
			final = set_bounds(cs1,rs2,choice);
			regs[instr.rd()] = final.cd.addr;
			regs.cheri[instr.rd()] = final.cd.caps;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

		}
		break;


		case Opcode::CSETHIGH: 
			REQUIRE_ISA(CHERI_ISA_EXT);
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			regs.cheri[instr.rd()] = (static_cast<uint32_t>(regs[instr.rs2()])) ^ ((NULL_CAP) & 0xFFFFFFFF);
			regs[instr.rd()] = static_cast<uint32_t>(regs[instr.rs1()]);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

			break;


		case Opcode::CCLEARTAG: 
			REQUIRE_ISA(CHERI_ISA_EXT);
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			regs[instr.rd()] = static_cast<uint32_t>(regs[instr.rs1()]);
			regs.cheri[instr.rd()] = regs.cheri[instr.rs1()];

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

			break;


		case Opcode::CBUILDCAP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1, cs2, cd;
			if ( (uint8_t)(instr.rs1()) == 0 ) {
				cs1.addr = pre_ddc;
				cs1.caps = cpre_ddc;
				cs1.tag = pre_ddc_tag;
			}
			else {
				cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
				cs1.caps = regs.cheri[instr.rs1()];
				cs1.tag = (BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0);
			}

			cs2.addr = static_cast<uint32_t>(regs[instr.rs2()]);
			cs2.caps = regs.cheri[instr.rs2()];
			
			if (BIT_SLICE(cs1.caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT) != Unsealed)
				cs1.tag = false;

			cd = build_cap(cs1, cs2);
			regs[instr.rd()] = cd.addr;
			regs.cheri[instr.rd()] = cd.caps;
			if (cd.tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		}
		break;


		case Opcode::CCOPYTYPE: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;
			uint8_t shift;
			int32_t rep_addr;
			bool fast_rep_check, neu_tag = 1;
			uint32_t cs2_caps = regs.cheri[instr.rs2()];

			int32_t otype = BIT_SLICE(cs2_caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT);
			if (otype <= Reserved) 
				rep_addr = otype;
			else {
				// Sign extend the otype value to fit into the target register
				shift = ADDR_BITS - (CAP_OTYPE_TOP - CAP_OTYPE_BOT + 1);
				rep_addr = (int32_t)(otype << shift) >> shift;
			}

			// uint32_t rep_addr = BIT_SLICE(cs2_caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT);
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			int64_t offset = static_cast<int64_t>(rep_addr) - static_cast<int64_t>(cs1.addr);
			cs1.caps = regs.cheri[instr.rs1()];
			uint8_t cs1_otype = BIT_SLICE((cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT); 
			
			if ((cs1_otype != Unsealed) || (BIT_SINGLE( (regs.tags), (instr.rs1()) ) == 0)) 
				neu_tag = 0;
			if (otype >= Reserved) neu_tag = 0; //if cs2 has a reserved otype
			if (neu_tag) {
				fast_rep_check = set_offset(cs1, offset, false);

				if (fast_rep_check) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
				else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			}
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			
			regs[instr.rd()] = rep_addr;
			regs.cheri[instr.rd()] = cs1.caps;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		}
		break;


		case Opcode::CCSEAL: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			uint32_t cs2_addr = static_cast<uint32_t>(regs[instr.rs2()]);
			uint32_t cs2_caps = regs.cheri[instr.rs2()];
			uint32_t cs1_caps = regs.cheri[instr.rs1()];
			bool cs2_tag = ( BIT_SINGLE( (regs.tags), (instr.rs2()) ) > 0 );
			uint32_t cs2_base = get_base(cs2_addr, cs2_caps);
			int64_t cs2_top = get_top(cs2_addr, cs2_caps);
			uint8_t cs1_otype = BIT_SLICE(cs1_caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT);

			bool passthrough = not(cs2_tag) || (cs1_otype != Unsealed) || (cs2_addr < cs2_base) || \
							   (cs2_addr >= cs2_top) || (cs2_addr == Unsealed);

			if (passthrough) {
				regs[instr.rd()] = static_cast<uint32_t>(regs[instr.rs1()]);
				regs.cheri[instr.rd()] = cs1_caps;
				if ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 ) 
					regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
				else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			}

			else {
				uint8_t cs2_otype = BIT_SLICE(cs2_caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT);
				uint32_t cs2_perms = BIT_SLICE(cs2_caps, CAP_PERM_TOP, CAP_PERM_BOT);
				bool permitted = (cs2_otype == Unsealed) & (BIT_SINGLE(cs2_perms, PERMIT_SEAL) > 0) & \
								 (cs2_addr < MAX_CAP_OTYPE);
				
				if ( (permitted) & ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 ) )
					regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
				else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
				
				regs[instr.rd()] = static_cast<uint32_t>(regs[instr.rs1()]);
				//Change otype of cs1 with addr of cs2
				cs1_caps = cs1_caps & REPLACEMENT_MASK(CAP_OTYPE_BOT, CAP_OTYPE_TOP);
				cs1_caps |= (cs2_addr << CAP_OTYPE_BOT);
				regs.cheri[instr.rd()] = cs1_caps;
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		}
		break;


		case Opcode::CSEALENTRY: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			uint32_t cs1_caps = regs.cheri[instr.rs1()];
			uint8_t cs1_otype = BIT_SLICE(cs1_caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT);

			if ((cs1_otype != Unsealed) || (BIT_SINGLE( (regs.tags), (instr.rs1()) ) == 0))
				regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = SET_BIT( (regs.tags), (instr.rd()) );

			regs[instr.rd()] = static_cast<uint32_t>(regs[instr.rs1()]);
			//Change otype of cs1 with Sentry
			cs1_caps = cs1_caps & REPLACEMENT_MASK(CAP_OTYPE_BOT, CAP_OTYPE_TOP);
			cs1_caps |= (Sealed << CAP_OTYPE_BOT);
			regs.cheri[instr.rd()] = cs1_caps;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

		}
		break;


		case Opcode::CMOVE:
			REQUIRE_ISA(CHERI_ISA_EXT);
			regs[instr.rd()] = regs[instr.rs1()];
			regs.cheri[instr.rd()] = regs.cheri[instr.rs1()];
			if (BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0) 
				regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
			else
				regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

			break;

// Deprecated instructions below added for TestRIG-----------------------------------------
#ifdef TESTRIG
		case Opcode::CTOPTR: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1, cs2;
			if (instr.rs2() == 0) {
				cs2.addr = pre_ddc;
				cs2.caps = cpre_ddc;
			}
			else {
				cs2.addr = static_cast<uint32_t>(regs[instr.rs2()]);
				cs2.caps = regs.cheri[instr.rs2()];
			}

			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = (BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0);

			if (not(cs1.tag)) regs[instr.rd()] = 0;
			else regs[instr.rd()] = cs1.addr - get_base(cs2.addr, cs2.caps);
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
		}
			break;


		case Opcode::CFROMPTR: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1, cd;
			uint8_t cs1_otype;
			int64_t offset;
			bool fast_rep_check;

			if (instr.rs1() == 0) {
				cs1.addr = pre_ddc;
				cs1.caps = cpre_ddc;
				cs1.tag = pre_ddc_tag;
			}
			else {
				cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
				cs1.caps = regs.cheri[instr.rs1()];
				cs1.tag = (BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0);
			}
			cs1_otype = BIT_SLICE(cs1.caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT);

			if (regs[instr.rs2()] == 0) {
				cd.addr = 0;
				cd.caps = NULL_CAP;
				cd.tag = false;
			}
			else {
				offset = static_cast<int64_t>(regs[instr.rs2()]);
				fast_rep_check = set_offset(cs1, offset, true);
				cd.addr = static_cast<uint32_t>(get_base(cs1.addr, cs1.caps) + offset);
				cd.caps = cs1.caps;
				if ((cs1.tag) && (cs1_otype == Unsealed) && fast_rep_check)
					cd.tag = true;
				else
					cd.tag = false;
			}
			
			regs[instr.rd()] = cd.addr;
			regs.cheri[instr.rd()] = cd.caps;
			if (cd.tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
		}
			break;


		case Opcode::CSUB: 
			REQUIRE_ISA(CHERI_ISA_EXT);
			regs[instr.rd()] = regs[instr.rs1()] - regs[instr.rs2()];
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

			break;
#endif
// Deprecated instructions-----------------------------------------------------------------


		case Opcode::CTESTSUBSET: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			uint32_t cs1_perms, cs2_perms, cs1_base, cs2_base;
			int64_t cs1_top, cs2_top;
			uint8_t cs1_otype, cs2_otype;
			Capability cs1, cs2;

			if ( (uint8_t)(instr.rs1()) == 0 ) {
				cs1.addr = pre_ddc;
				cs1.caps = cpre_ddc;
				cs1.tag = pre_ddc_tag;
			}
			else {
				cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
				cs1.caps = regs.cheri[instr.rs1()];
				cs1.tag = (BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0);
			}

			cs2.addr = static_cast<uint32_t>(regs[instr.rs2()]);
			cs2.caps = regs.cheri[instr.rs2()];
			cs2.tag = (BIT_SINGLE( (regs.tags), (instr.rs2()) ) > 0);
			
			cs1_base = get_base(cs1.addr, cs1.caps);
			cs2_base = get_base(cs2.addr, cs2.caps);
			
			cs1_top = get_top(cs1.addr, cs1.caps);
			cs2_top = get_top(cs2.addr, cs2.caps);

			cs1_perms = BIT_SLICE( (cs1.caps), CAP_PERM_TOP, CAP_PERM_BOT );
			cs2_perms = BIT_SLICE( (cs2.caps), CAP_PERM_TOP, CAP_PERM_BOT );

			cs1_otype = BIT_SLICE( (cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT );
			cs2_otype = BIT_SLICE( (cs2.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT );

			if ((cs1.tag) != (cs2.tag)) regs[instr.rd()] = 0b0;
			else if (cs1_otype != cs2_otype) regs[instr.rd()] = 0b0; //distinct otype means unordered
			else if (cs2_base < cs1_base) regs[instr.rd()] = 0b0;
			else if (cs2_top > cs1_top) regs[instr.rd()] = 0b0;
			else if ( (cs2_perms & cs1_perms) != cs2_perms ) regs[instr.rd()] = 0b0;
			else regs[instr.rd()] = 0b1;
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		}
		break;


		case Opcode::CSETEQUALEXACT: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			uint32_t cs1_addr = regs[instr.rs1()];
			uint32_t cs2_addr = regs[instr.rs2()];

			uint32_t cs1_caps = regs.cheri[instr.rs1()];
			uint32_t cs2_caps = regs.cheri[instr.rs2()];

			bool cs1_tag = (BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0);
			bool cs2_tag = (BIT_SINGLE( (regs.tags), (instr.rs2()) ) > 0);

			if ((cs1_tag) != (cs2_tag)) regs[instr.rd()] = 0b0;
			else if (cs1_addr != cs2_addr) regs[instr.rd()] =  0b0;
			else if (cs1_caps != cs2_caps) regs[instr.rd()] = 0b0;
			else regs[instr.rd()] =  0b1;
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		}
		break;


		case Opcode::CCLEAR: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			uint8_t q = instr.q();
			uint8_t m_upper = instr.m_upper();
			uint8_t m_lower = instr.m_lower();

			uint8_t m = (static_cast<uint8_t>(m_upper) << 5) | m_lower;
			for (int i=0; i<8; i++){
				if (BIT_SINGLE(m, i) > 0) {
					if ((q == 0) && (i == 0)) {
						pre_ddc = 0;
						cpre_ddc = 0;
						cpre_ddc |= NULL_CAP;
						pre_ddc_tag = 0;
					}
					else {
						uint8_t regnum = 8*q + i;
						regs[regnum] = 0;
						regs.cheri[regnum] = 0;
						regs.cheri[regnum] |= NULL_CAP;
						regs.tags = UNSET_BIT( (regs.tags), regnum );
					}
				}
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

		} break;


		case Opcode::CROUNDREPLEN: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs;
			SetBound_Ret final;
			SetBound_flag choice;

			cs.addr = pre_ddc;
			cs.caps = cpre_ddc;
			cs.tag = pre_ddc_tag;
			uint32_t newlen = ((uint32_t)regs[instr.rs1()]);

			choice.len = true;
			final = set_bounds(cs, newlen, choice);

			regs[instr.rd()] = final.newlen;
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

		} break;


		case Opcode::CREPALIGNMASK: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs;
			SetBound_Ret final;
			SetBound_flag choice;

			cs.addr = pre_ddc;
			cs.caps = cpre_ddc;
			cs.tag = pre_ddc_tag;
			uint32_t newlen = ((uint32_t)regs[instr.rs1()]);

			choice.mask = true;
			final = set_bounds(cs, newlen, choice);

#ifdef TESTRIG
			// For CHERI capability structure in TestRIG, the masks differ
			// The mask calculated by us is not wrong
			if (final.baseMask != (~0u)) final.baseMask = final.baseMask << 1;
#endif
			regs[instr.rd()] = final.baseMask;
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
#endif

		} break;
/*spandan: CHERI cap read + modify--------------------------------------------------------------------*/



/*spandan-----------------------------------------------------*/
// AUIPC works differently in integer mode vs capability mode.
		case Opcode::AUIPC: {
if (BIT_SINGLE(cpc, CAP_FLAG) == 0) {
			regs[instr.rd()] = last_pc + instr.U_imm();
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
} else {
			Capability cd;

			cd.addr = last_pc;
			cd.caps = cpc;
			cd.tag = pc_tag;
			int32_t offset = instr.U_imm();

			bool fast_rep_check = set_offset(cd, offset, false);
			regs[instr.rd()] = static_cast<uint32_t>(static_cast<int64_t>(cd.addr) + offset);
			regs.cheri[instr.rd()] = cd.caps;

			if (not(cd.tag) || not(fast_rep_check)) regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif
		} break;
/*spandan-----------------------------------------------------*/



/*spandan-----------------------------------------------------*/
// JAL works differently in integer mode vs capability mode.
		case Opcode::JAL: {		// Same as CJAL
if (BIT_SINGLE(cpc, CAP_FLAG) == 0) {
			auto link = pc;
			pc = last_pc + instr.J_imm();
			trap_check_pc_alignment();
			regs[instr.rd()] = link;
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

} else {
			Capability cd;

			uint32_t link = pc;
			cd.addr = last_pc;
			cd.caps = cpc;
			cd.tag = pc_tag;
			uint8_t cd_otype = BIT_SLICE( (cd.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT);
			int32_t offset = (instr.J_imm());

			check_jal_exceptions(32, cd, offset);
			pc = static_cast<uint32_t>(static_cast<int64_t>(last_pc) + offset);
			trap_check_pc_alignment();

			int64_t pc_offset = (int64_t)link - last_pc;
			bool fast_rep_check = set_offset(cd, pc_offset, false);

#if defined(NOTAGROOT)
			if ( not(fast_rep_check) || (cd_otype != Unsealed) ) {
				std::cout << "[assert] JAL : Link cap not Unsealed or not representable" << std::endl;
				raise_trap(EXC_INSTR_ACCESS_FAULT, instr.data()); //Non-CHERI exception for ease
			}
#else
			assert(fast_rep_check && "Link cap should always be representable");
			assert( (cd_otype == Unsealed) && "Link cap should always be unsealed");
#endif
			cd.addr = link;

			regs[instr.rd()] = (cd.addr);
			regs.cheri[instr.rd()] = replace_bits(cd.caps, CAP_OTYPE_BOT, CAP_OTYPE_TOP, Sealed);
			if (cd.tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif
		} break;
/*spandan-----------------------------------------------------*/	



/*spandan-----------------------------------------------------*/
// JALR works differently in integer mode vs capability mode.
		case Opcode::JALR: {
if (BIT_SINGLE(cpc, CAP_FLAG) == 0) {
			auto link = pc;
			pc = (regs[instr.rs1()] + instr.I_imm()) & ~1;
			trap_check_pc_alignment();
			regs[instr.rd()] = link;
			/*spandan--------------------*/
			//When target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

} else {
			Capability cs1, cd;

			auto link = pc;
			auto clink = cpc;
			bool link_tag = pc_tag;
			int32_t offset = (instr.I_imm());
			pc = static_cast<uint32_t>(static_cast<int64_t>(regs[instr.rs1()]) + offset) & ~1;

			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = (BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0);
			uint8_t cs1_otype = BIT_SLICE( (cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT);

			trap_check_pc_alignment();
			check_jalr_exceptions((RS1), cs1, offset);

			bool fast_rep_check;
			cd.addr = last_pc;
			cd.caps = clink;
			uint8_t cd_otype = BIT_SLICE((cd.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT); 
			
			int64_t pc_offset = (int64_t)link - last_pc;
			fast_rep_check = set_offset(cd, pc_offset, false);

#if defined(NOTAGROOT)
			if ( not(fast_rep_check) || (cd_otype != Unsealed) ) {
				std::cout << "[assert] JALR : Link cap not Unsealed or not representable" << std::endl;
				raise_trap(EXC_INSTR_ACCESS_FAULT, instr.data()); //Non-CHERI exception for ease
			}
#else
			assert( (fast_rep_check) && "Link capability should always be representable" );
			assert( (cd_otype == Unsealed) && "Link capability should always be unsealed" );
#endif

			cd.addr = link;
			
			regs[instr.rd()] = (cd.addr);
			regs.cheri[instr.rd()] = replace_bits(cd.caps, CAP_OTYPE_BOT, CAP_OTYPE_TOP, Sealed);
			if (link_tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

			if (cs1_otype == Sealed) cpc = replace_bits( (cs1.caps), CAP_OTYPE_BOT, CAP_OTYPE_TOP, Unsealed );
			else cpc = cs1.caps;

			pc_tag = cs1.tag;
		}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif
		} break;
/*spandan-----------------------------------------------------*/


/*spandan-----------------------------------------------------*/
		case Opcode::JALRCAP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1, cd;

			auto link = pc;
			auto clink = cpc;
			bool link_tag = pc_tag;
			pc = static_cast<uint32_t>(regs[instr.rs1()]) & ~1;

			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = (BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0);
			uint8_t cs1_otype = BIT_SLICE( (cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT);

			trap_check_pc_alignment();
			check_jalr_exceptions((RS1), cs1, 0);

			bool fast_rep_check;
			cd.addr = last_pc;
			cd.caps = clink;
			uint8_t cd_otype = BIT_SLICE((cd.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT); 
			
			int64_t pc_offset = (int64_t)link - last_pc;
			fast_rep_check = set_offset(cd, pc_offset, false);

#if defined(NOTAGROOT)
			if ( not(fast_rep_check) || (cd_otype != Unsealed) ) {
				std::cout << "[assert] JALRCAP : Link cap not Unsealed or not representable" << std::endl;
				raise_trap(EXC_INSTR_ACCESS_FAULT, instr.data()); //Non-CHERI exception for ease 
			}
#else
			assert( (fast_rep_check) && "Link capability should always be representable" );
			assert( (cd_otype == Unsealed) && "Link capability should always be unsealed" );
#endif

			cd.addr = link;
			
			regs[instr.rd()] = (cd.addr);
			regs.cheri[instr.rd()] = replace_bits(cd.caps, CAP_OTYPE_BOT, CAP_OTYPE_TOP, Sealed);
			if (link_tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

			if (cs1_otype == Sealed) cpc = replace_bits( (cs1.caps), CAP_OTYPE_BOT, CAP_OTYPE_TOP, Unsealed );
			else cpc = cs1.caps;

			pc_tag = cs1.tag;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_pc_wdata = pc;
#endif

		} break;

		case Opcode::JALRPCC: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;

			auto link = pc;
			auto clink = cpc;
			bool link_tag = pc_tag;
			uint32_t link_base = get_base(link, clink);
			int32_t link_offset = static_cast<int32_t>(static_cast<int64_t>(link) - static_cast<int64_t>(link_base));

			cs1.addr = link_base;
			cs1.caps = clink;
			cs1.tag = link_tag;
			uint32_t offset = regs[instr.rs1()];

			//If PCC raises exception with offset rs1
			check_jalr_exceptions(32, cs1, offset);
			pc = static_cast<uint32_t>(static_cast<int64_t>(link_base) + offset) & ~1;
			trap_check_pc_alignment();
			
			regs[instr.rd()] = link_offset;
			/*spandan--------------------*/
			//If target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_pc_wdata = pc;
#endif

		} break;

		case Opcode::CINVOKE: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1, cs2;

			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = (BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0);

			cs2.addr = static_cast<uint32_t>(regs[instr.rs2()]);
			cs2.caps = regs.cheri[instr.rs2()];
			cs2.tag = (BIT_SINGLE( (regs.tags), (instr.rs2()) ) > 0);

			check_invoke_exceptions((RS1), (RS2), cs1, cs2);
			pc = (cs1.addr) & ~1;
			trap_check_pc_alignment();

			regs[31] = cs2.addr;
			regs.cheri[31] = replace_bits((cs2.caps), CAP_OTYPE_BOT, CAP_OTYPE_TOP, Unsealed);
			if (cs2.tag) regs.tags = SET_BIT( (regs.tags), 31 );
			else regs.tags = UNSET_BIT( (regs.tags), 31 );

			cpc = replace_bits((cs1.caps), CAP_OTYPE_BOT, CAP_OTYPE_TOP, Unsealed);
			pc_tag = cs1.tag;

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_pc_wdata = pc;
#endif

		} break;
/*spandan-----------------------------------------------------*/

/*spandan-----------------------------------------------------*/
		case Opcode::SD: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			//Same effect as [C]SC
			Capability cs1,cs2;
			uint64_t str_addr;
			int64_t imm;

if (BIT_SINGLE(cpc, CAP_FLAG)) {
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );
			imm = (instr.S_imm());
} else {
			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			imm = (instr.S_imm()) + regs[instr.rs1()];
}

			cs2.addr = static_cast<uint32_t>(regs[instr.rs2()]);
			cs2.caps = regs.cheri[instr.rs2()];
			cs2.tag = ( BIT_SINGLE( (regs.tags), (instr.rs2()) ) > 0 );

			// int32_t imm = (instr.S_imm());
			check_csc_exceptions( (RS1), (RS2), cs1, cs2, imm );
			str_addr = static_cast<uint64_t>(static_cast<int64_t>(cs1.addr) + imm);
			//memory alignment exceptions are handled in dmi

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(str_addr);
#if defined(NOTAGROOT)
			if ( str_addr % (BYTES_PER_TAG) ) {
				std::cout << "Alignment error at address " << std::to_string(str_addr) << std::endl;
				raise_trap(EXC_STORE_AMO_ACCESS_FAULT, static_cast<uint32_t>(str_addr)); //Non-CHERI exception for ease
			}

			if ((str_addr >= 0x80000000)  && (str_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif

			mem->store_capability(effective_addr, cs2);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_mem_addr = str_addr;
			rvfi_dii_output.rvfi_dii_mem_wmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_wdata = static_cast<uint64_t>(cs2.addr);
#endif

		} break;

		case Opcode::SCCAP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1,cs2;
			uint64_t str_addr;


			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );

			cs2.addr = static_cast<uint32_t>(regs[instr.rs2()]);
			cs2.caps = regs.cheri[instr.rs2()];
			cs2.tag = ( BIT_SINGLE( (regs.tags), (instr.rs2()) ) > 0 );

			check_csc_exceptions( (RS1), (RS2), cs1, cs2, 0);
			str_addr = static_cast<uint64_t>(cs1.addr);
			//memory alignment exceptions are handled in dmi

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(str_addr);
#if defined(NOTAGROOT)
			if ( str_addr % (BYTES_PER_TAG) ) {
				std::cout << "Alignment error at address " << std::to_string(str_addr) << std::endl;
				raise_trap(EXC_STORE_AMO_ACCESS_FAULT, static_cast<uint32_t>(str_addr)); //Non-CHERI exception for ease
			}

			if ((str_addr >= 0x80000000)  && (str_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif

			mem->store_capability(effective_addr, cs2);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_mem_addr = str_addr;
			rvfi_dii_output.rvfi_dii_mem_wmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_wdata = static_cast<uint64_t>(cs2.addr);
#endif

		} break;

		case Opcode::SCDDC: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1,cs2;
			uint64_t str_addr;


			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;

			cs2.addr = static_cast<uint32_t>(regs[instr.rs2()]);
			cs2.caps = regs.cheri[instr.rs2()];
			cs2.tag = ( BIT_SINGLE( (regs.tags), (instr.rs2()) ) > 0 );

			uint32_t imm = static_cast<uint32_t>(regs[instr.rs1()]);
			check_csc_exceptions( 33, (RS2), cs1, cs2, imm );
			str_addr = static_cast<uint64_t>(static_cast<int64_t>(cs1.addr) + imm);
			//memory alignment exceptions are handled in dmi

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(str_addr);
#if defined(NOTAGROOT)
			if ( str_addr % (BYTES_PER_TAG) ) {
				std::cout << "Alignment error at address " << std::to_string(str_addr) << std::endl;
				raise_trap(EXC_STORE_AMO_ACCESS_FAULT, static_cast<uint32_t>(str_addr)); //Non-CHERI exception for ease
			}

			if ((str_addr >= 0x80000000)  && (str_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif

			mem->store_capability(effective_addr, cs2);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_mem_addr = str_addr;
			rvfi_dii_output.rvfi_dii_mem_wmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_wdata = static_cast<uint64_t>(cs2.addr);
#endif

		} break;

		case Opcode::SBCAP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;


			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );
			
			check_datastore_exceptions( (RS1), cs1, 1, 0);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(cs1.addr);
#if defined(NOTAGROOT)
			if ((cs1.addr >= 0x80000000)  && (cs1.addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			mem->store_byte(effective_addr, regs[instr.rs2()]);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_mem_addr = cs1.addr;
			rvfi_dii_output.rvfi_dii_mem_wmask = 0b00000001;
			rvfi_dii_output.rvfi_dii_mem_wdata = static_cast<uint64_t>(static_cast<uint8_t>(regs[instr.rs2()]));
#endif

		} break;

		case Opcode::SHCAP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;


			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );
			
			check_datastore_exceptions( (RS1), cs1, 2, 0);
			trap_check_addr_alignment<2, false>(cs1.addr);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(cs1.addr);
#if defined(NOTAGROOT)
			if ((cs1.addr >= 0x80000000)  && (cs1.addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			mem->store_half(effective_addr, regs[instr.rs2()]);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_mem_addr = cs1.addr;
			rvfi_dii_output.rvfi_dii_mem_wmask = 0b00000011;
			rvfi_dii_output.rvfi_dii_mem_wdata = static_cast<uint64_t>(static_cast<uint16_t>(regs[instr.rs2()]));
#endif

		} break;

		case Opcode::SWCAP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;


			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );
			
			check_datastore_exceptions( (RS1), cs1, 4, 0);
			trap_check_addr_alignment<4, false>(cs1.addr);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(cs1.addr);
#if defined(NOTAGROOT)
			if ((cs1.addr >= 0x80000000)  && (cs1.addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			mem->store_word(effective_addr, regs[instr.rs2()]);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_mem_addr = cs1.addr;
			rvfi_dii_output.rvfi_dii_mem_wmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		} break;

		case Opcode::SBDDC: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;


			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			uint32_t imm = static_cast<uint32_t>(regs[instr.rs1()]);
			
			check_datastore_exceptions( 33, cs1, 1, imm );
			uint64_t str_addr = static_cast<uint64_t>(static_cast<int64_t>(cs1.addr) + imm);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(str_addr);
#if defined(NOTAGROOT)
			if ((str_addr >= 0x80000000)  && (str_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			mem->store_byte(effective_addr, regs[instr.rs2()]);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_mem_addr = str_addr;
			rvfi_dii_output.rvfi_dii_mem_wmask = 0b00000001;
			rvfi_dii_output.rvfi_dii_mem_wdata = static_cast<uint64_t>(static_cast<uint8_t>(regs[instr.rs2()]));
#endif

		} break;

		case Opcode::SHDDC: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;


			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			uint32_t imm = static_cast<uint32_t>(regs[instr.rs1()]);
			
			check_datastore_exceptions( 33, cs1, 2, imm );
			uint64_t str_addr = static_cast<uint64_t>(static_cast<int64_t>(cs1.addr) + imm);
			trap_check_addr_alignment<2, false>(str_addr);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(str_addr);
#if defined(NOTAGROOT)
			if ((str_addr >= 0x80000000)  && (str_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			mem->store_half(effective_addr, regs[instr.rs2()]);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_mem_addr = str_addr;
			rvfi_dii_output.rvfi_dii_mem_wmask = 0b00000011;
			rvfi_dii_output.rvfi_dii_mem_wdata = static_cast<uint64_t>(static_cast<uint16_t>(regs[instr.rs2()]));
#endif

		} break;

		case Opcode::SWDDC: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;


			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			uint32_t imm = static_cast<uint32_t>(regs[instr.rs1()]);
			
			check_datastore_exceptions( 33, cs1, 4, imm );
			uint64_t str_addr = static_cast<uint64_t>(static_cast<int64_t>(cs1.addr) + imm);
			trap_check_addr_alignment<4, false>(str_addr);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(str_addr);
#if defined(NOTAGROOT)
			if ((str_addr >= 0x80000000)  && (str_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			mem->store_word(effective_addr, regs[instr.rs2()]);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_mem_addr = str_addr;
			rvfi_dii_output.rvfi_dii_mem_wmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif

		} break;
/*spandan-----------------------------------------------------*/

/*spandan-----------------------------------------------------*/
		case Opcode::SB: {
			Capability cs1;
			uint32_t addr = regs[instr.rs1()] + instr.S_imm();

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(addr);

			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			int64_t offset = static_cast<int64_t>(addr) - static_cast<int64_t>(cs1.addr);

			check_datastore_exceptions( 33, cs1, 1, offset );

#if defined(NOTAGROOT)
			if ((addr >= 0x80000000)  && (addr < 0x80010000)) 
				{effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			mem->store_byte(effective_addr, regs[instr.rs2()]);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_mem_addr = addr;
			rvfi_dii_output.rvfi_dii_mem_wmask = 0b00000001;
			rvfi_dii_output.rvfi_dii_mem_wdata = static_cast<uint64_t>(static_cast<uint8_t>(regs[instr.rs2()]));
#endif
		} break;

		case Opcode::SH: {
			Capability cs1;
			uint32_t addr = regs[instr.rs1()] + instr.S_imm();

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(addr);

			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			int64_t offset = static_cast<int64_t>(addr) - static_cast<int64_t>(cs1.addr);

			check_datastore_exceptions( 33, cs1, 2, offset );
			trap_check_addr_alignment<2, false>(addr);

#if defined(NOTAGROOT)
			if ((addr >= 0x80000000)  && (addr < 0x80010000)) 
				{effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			mem->store_half(effective_addr, regs[instr.rs2()]);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_mem_addr = addr;
			rvfi_dii_output.rvfi_dii_mem_wmask = 0b00000011;
			rvfi_dii_output.rvfi_dii_mem_wdata = static_cast<uint64_t>(static_cast<uint16_t>(regs[instr.rs2()]));
#endif
		} break;

		case Opcode::SW: {
			Capability cs1;
			uint32_t addr = regs[instr.rs1()] + instr.S_imm();

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(addr);

			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			int64_t offset = static_cast<int64_t>(addr) - static_cast<int64_t>(cs1.addr);

			check_datastore_exceptions( 33, cs1, 4, offset );
			trap_check_addr_alignment<4, false>(addr);

#if defined(NOTAGROOT)
			if ((addr >= 0x80000000)  && (addr < 0x80010000)) 
				{effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			mem->store_word(effective_addr, regs[instr.rs2()]);

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
			rvfi_dii_output.rvfi_dii_mem_addr = addr;
			rvfi_dii_output.rvfi_dii_mem_wmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
		} break;
/*spandan-----------------------------------------------------*/

/*spandan------------------------------------------------------*/
		case Opcode::CLOADTAGS: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;

			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );

			check_ltags_exceptions( (RS1), cs1 );
			//memory alignment exceptions are handled in dmi

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(cs1.addr);

#if defined(NOTAGROOT)
			if ((cs1.addr >= 0x80000000) && (cs1.addr < 0x80010000)) 
				{effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			uint32_t tags = mem->load_tags(effective_addr);

			regs[instr.rd()] = tags;
			/*spandan--------------------*/
			//If target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = cs1.addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

		case Opcode::LD: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			//Same effect as [C]LC
			Capability cs1, cd;
			uint64_t ld_addr;
			int64_t imm;

if (BIT_SINGLE(cpc, CAP_FLAG)) {
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );
			imm = (instr.I_imm());
} else {
			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			imm = (instr.I_imm()) + regs[instr.rs1()];
}
			
			uint32_t cs1_perms = BIT_SLICE( (cs1.caps), CAP_PERM_TOP, CAP_PERM_BOT);
			// int32_t imm = (instr.I_imm());

			check_clc_exceptions( (RS1), cs1, imm);
			ld_addr = static_cast<uint64_t>(static_cast<int64_t>(cs1.addr) + imm);
			//memory alignment exceptions are handled in dmi

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(ld_addr);
#if defined(NOTAGROOT)
			if ( ld_addr % (BYTES_PER_TAG) ) {
				std::cout << "Alignment error at address " << std::to_string(ld_addr) << std::endl;
				raise_trap(EXC_LOAD_ACCESS_FAULT, static_cast<uint32_t>(ld_addr)); //Non-CHERI exception for ease
			}

			if ((ld_addr >= 0x80000000) && (ld_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif

			cd = mem->load_capability(effective_addr);

			if (BIT_SINGLE(cs1_perms, PERMIT_LOAD_CAP) == 0) cd.tag = false;
			regs[instr.rd()] = cd.addr;
			regs.cheri[instr.rd()] = cd.caps;
			if (cd.tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = ld_addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(cd.addr);
#endif

		} break;

		case Opcode::LCCAP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1, cd;
			uint64_t ld_addr;


			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );
			uint32_t cs1_perms = BIT_SLICE( (cs1.caps), CAP_PERM_TOP, CAP_PERM_BOT);

			check_clc_exceptions( (RS1), cs1, 0);
			ld_addr = static_cast<uint64_t>(cs1.addr);
			//memory alignment exceptions are handled in dmi

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(ld_addr);
#if defined(NOTAGROOT)
			if ( ld_addr % (BYTES_PER_TAG) ) {
				std::cout << "Alignment error at address " << std::to_string(ld_addr) << std::endl;
				raise_trap(EXC_LOAD_ACCESS_FAULT, static_cast<uint32_t>(ld_addr)); //Non-CHERI exception for ease
			}

			if ((ld_addr >= 0x80000000) && (ld_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif

			cd = mem->load_capability(effective_addr);

			if (BIT_SINGLE(cs1_perms, PERMIT_LOAD_CAP) == 0) cd.tag = false;
			regs[instr.rd()] = cd.addr;
			regs.cheri[instr.rd()] = cd.caps;
			if (cd.tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = ld_addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(cd.addr);
#endif

		} break;

		case Opcode::LCDDC: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1, cd;
			uint64_t ld_addr;



			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			uint32_t cs1_perms = BIT_SLICE( (cs1.caps), CAP_PERM_TOP, CAP_PERM_BOT);
			uint32_t imm = static_cast<uint32_t>(regs[instr.rs1()]);

			check_clc_exceptions( 33, cs1, imm );
			ld_addr = static_cast<uint64_t>(static_cast<int64_t>(cs1.addr) + imm);
			//memory alignment exceptions are handled in dmi

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(ld_addr);
#if defined(NOTAGROOT)
			if ( ld_addr % (BYTES_PER_TAG) ) {
				std::cout << "Alignment error at address " << std::to_string(ld_addr) << std::endl;
				raise_trap(EXC_LOAD_ACCESS_FAULT, static_cast<uint32_t>(ld_addr)); //Non-CHERI exception for ease
			}

			if ((ld_addr >= 0x80000000) && (ld_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif

			cd = mem->load_capability(effective_addr);

			if (BIT_SINGLE(cs1_perms, PERMIT_LOAD_CAP) == 0) cd.tag = false;
			regs[instr.rd()] = cd.addr;
			regs.cheri[instr.rd()] = cd.caps;
			if (cd.tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
			else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = ld_addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(cd.addr);
#endif

		} break;

		case Opcode::LBCAP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;

			
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );
			
			check_dataload_exceptions( (RS1), cs1, 1, 0 );

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(cs1.addr);
#if defined(NOTAGROOT)
			if ((cs1.addr >= 0x80000000) && (cs1.addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			regs[instr.rd()] = mem->load_byte(effective_addr);
			//Not sure if needed
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = cs1.addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00000001;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

		case Opcode::LHCAP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;

			
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );
			
			check_dataload_exceptions( (RS1), cs1, 2, 0 );
			trap_check_addr_alignment<2, true>(cs1.addr);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(cs1.addr);
#if defined(NOTAGROOT)
			if ((cs1.addr >= 0x80000000) && (cs1.addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			regs[instr.rd()] = mem->load_half(effective_addr);
			//Not sure if needed
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = cs1.addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00000011;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

		case Opcode::LWCAP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;

			
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );
			
			check_dataload_exceptions( (RS1), cs1, 4, 0 );
			trap_check_addr_alignment<4, true>(cs1.addr);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(cs1.addr);
#if defined(NOTAGROOT)
			if ((cs1.addr >= 0x80000000) && (cs1.addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			regs[instr.rd()] = mem->load_word(effective_addr);
			//Not sure if needed
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = cs1.addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

		case Opcode::LBUCAP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;

			
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );
			
			check_dataload_exceptions( (RS1), cs1, 1, 0 );

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(cs1.addr);
#if defined(NOTAGROOT)
			if ((cs1.addr >= 0x80000000) && (cs1.addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			regs[instr.rd()] = mem->load_ubyte(effective_addr);
			//Not sure if needed
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = cs1.addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00000001;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

		case Opcode::LHUCAP: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;

			
			cs1.addr = static_cast<uint32_t>(regs[instr.rs1()]);
			cs1.caps = regs.cheri[instr.rs1()];
			cs1.tag = ( BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0 );
			
			check_dataload_exceptions( (RS1), cs1, 2, 0 );
			trap_check_addr_alignment<2, true>(cs1.addr);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(cs1.addr);
#if defined(NOTAGROOT)
			if ((cs1.addr >= 0x80000000) && (cs1.addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			regs[instr.rd()] = mem->load_uhalf(effective_addr);
			//Not sure if needed
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = cs1.addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00000011;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

		case Opcode::LBDDC: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;

			
			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			uint32_t imm = static_cast<uint32_t>(regs[instr.rs1()]);
			
			check_dataload_exceptions( 33, cs1, 1, imm );

			uint32_t ld_addr = static_cast<uint32_t>(static_cast<int64_t>(cs1.addr) + imm);
			
			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(ld_addr);
#if defined(NOTAGROOT)
			if ((ld_addr >= 0x80000000) && (ld_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif

			regs[instr.rd()] = mem->load_byte(effective_addr);
			//Not sure if needed
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = ld_addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00000001;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

		case Opcode::LHDDC: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;

			
			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			uint32_t imm = static_cast<uint32_t>(regs[instr.rs1()]);
			
			check_dataload_exceptions( 33, cs1, 2, imm );
			uint32_t ld_addr = static_cast<uint32_t>(static_cast<int64_t>(cs1.addr) + imm);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(ld_addr);
#if defined(NOTAGROOT)
			if ((ld_addr >= 0x80000000) && (ld_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif

			trap_check_addr_alignment<2, true>(ld_addr);

			regs[instr.rd()] = mem->load_half(effective_addr);
			//Not sure if needed
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = ld_addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00000011;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

		case Opcode::LWDDC: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;

			
			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			uint32_t imm = static_cast<uint32_t>(regs[instr.rs1()]);
			
			check_dataload_exceptions( 33, cs1, 4, imm );
			uint32_t ld_addr = static_cast<uint32_t>(static_cast<int64_t>(cs1.addr) + imm);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(ld_addr);
#if defined(NOTAGROOT)
			if ((ld_addr >= 0x80000000) && (ld_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif

			trap_check_addr_alignment<4, true>(ld_addr);

			regs[instr.rd()] = mem->load_word(effective_addr);
			//Not sure if needed
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = ld_addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

		case Opcode::LBUDDC: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;

			
			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			uint32_t imm = static_cast<uint32_t>(regs[instr.rs1()]);
			
			check_dataload_exceptions( 33, cs1, 1, imm );

			uint32_t ld_addr = static_cast<uint32_t>(static_cast<int64_t>(cs1.addr) + imm);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(ld_addr);
#if defined(NOTAGROOT)
			if ((ld_addr >= 0x80000000) && (ld_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif

			regs[instr.rd()] = mem->load_ubyte(effective_addr);
			//Not sure if needed
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = ld_addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00000001;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

		case Opcode::LHUDDC: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			Capability cs1;

			
			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			uint32_t imm = static_cast<uint32_t>(regs[instr.rs1()]);
			
			check_dataload_exceptions( 33, cs1, 2, imm );
			uint32_t ld_addr = static_cast<uint32_t>(static_cast<int64_t>(cs1.addr) + imm);
			trap_check_addr_alignment<2, true>(ld_addr);

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(ld_addr);
#if defined(NOTAGROOT)
			if ((ld_addr >= 0x80000000) && (ld_addr < 0x80010000)) 
				{ effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			regs[instr.rd()] = mem->load_uhalf(effective_addr);
			//Not sure if needed
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = ld_addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00000011;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;
/*spandan------------------------------------------------------*/

/*spandan------------------------------------------------------*/
		case Opcode::LB: {
			Capability cs1;
			uint32_t addr = regs[instr.rs1()] + instr.I_imm();

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(addr);

			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			int64_t offset = static_cast<int64_t>(addr) - static_cast<int64_t>(cs1.addr);

			check_dataload_exceptions( 33, cs1, 1, offset );

#if defined(NOTAGROOT)
			if ((addr >= 0x80000000)  && (addr < 0x80010000)) 
				{effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			regs[instr.rd()] = mem->load_byte(effective_addr);
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00000001;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif
		} break;

		case Opcode::LH: {
			Capability cs1;
			uint32_t addr = regs[instr.rs1()] + instr.I_imm();

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(addr);

			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			int64_t offset = static_cast<int64_t>(addr) - static_cast<int64_t>(cs1.addr);

			check_dataload_exceptions( 33, cs1, 2, offset );
			trap_check_addr_alignment<2, true>(addr);

#if defined(NOTAGROOT)
			if ((addr >= 0x80000000)  && (addr < 0x80010000)) 
				{effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			regs[instr.rd()] = mem->load_half(effective_addr);
			/*spandan--------------------*/
			//If target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00000011;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif
		} break;

		case Opcode::LW: {
			Capability cs1;
			uint32_t addr = regs[instr.rs1()] + instr.I_imm();

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(addr);

			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			int64_t offset = static_cast<int64_t>(addr) - static_cast<int64_t>(cs1.addr);

			check_dataload_exceptions( 33, cs1, 4, offset );
			trap_check_addr_alignment<4, true>(addr);

#if defined(NOTAGROOT)
			if ((addr >= 0x80000000)  && (addr < 0x80010000)) 
				{effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			regs[instr.rd()] = mem->load_word(effective_addr);
			/*spandan--------------------*/
			//If target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00001111;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif
		} break;

		case Opcode::LBU: {
			Capability cs1;
			uint32_t addr = regs[instr.rs1()] + instr.I_imm();

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(addr);

			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			int64_t offset = static_cast<int64_t>(addr) - static_cast<int64_t>(cs1.addr);

			check_dataload_exceptions( 33, cs1, 1, offset );

#if defined(NOTAGROOT)
			if ((addr >= 0x80000000)  && (addr < 0x80010000)) 
				{effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			regs[instr.rd()] = mem->load_ubyte(effective_addr);
			/*spandan--------------------*/
			//If target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00000001;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif
		} break;

		case Opcode::LHU: {
			Capability cs1;
			uint32_t addr = regs[instr.rs1()] + instr.I_imm();

			//spandan : effective address changes for TestRIG
			uint64_t effective_addr = static_cast<uint64_t>(addr);

			cs1.addr = pre_ddc;
			cs1.caps = cpre_ddc;
			cs1.tag = pre_ddc_tag;
			int64_t offset = static_cast<int64_t>(addr) - static_cast<int64_t>(cs1.addr);

			check_dataload_exceptions( 33, cs1, 2, offset );
			trap_check_addr_alignment<2, true>(addr);

#if defined(NOTAGROOT)
			if ((addr >= 0x80000000)  && (addr < 0x80010000)) 
				{effective_addr -= 0x80000000; }
			else effective_addr += 0x80000000;
#endif
			regs[instr.rd()] = mem->load_uhalf(effective_addr);
			/*spandan--------------------*/
			//If target register is int, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_mem_addr = addr;
			rvfi_dii_output.rvfi_dii_mem_rmask = 0b00000011;
			rvfi_dii_output.rvfi_dii_mem_rdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif
		} break;
/*spandan------------------------------------------------------*/

		case Opcode::BEQ: {
			if (regs[instr.rs1()] == regs[instr.rs2()]) {
				Capability cs;
				cs.addr = last_pc;
				cs.caps = cpc;
				cs.tag = pc_tag;
				int64_t pc_offset = (int64_t)(instr.B_imm());
				bool fast_rep_check = set_offset(cs, pc_offset, false);

#ifdef NOTAGROOT
				if ( not(fast_rep_check) ) {
					std::cout << "[assert] BEQ : New PCC should always be representable" << std::endl;
					raise_trap(EXC_INSTR_ACCESS_FAULT, instr.data()); //Non-CHERI exception for ease 
				}
#else
				assert(fast_rep_check && "New PCC should always be representable");
#endif
				pc = last_pc + instr.B_imm();
				trap_check_pc_alignment();

#ifdef TESTRIG
				rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
		}
			break;

		case Opcode::BNE: {
			if (regs[instr.rs1()] != regs[instr.rs2()]) {
				Capability cs;
				cs.addr = last_pc;
				cs.caps = cpc;
				cs.tag = pc_tag;
				int64_t pc_offset = (int64_t)(instr.B_imm());
				bool fast_rep_check = set_offset(cs, pc_offset, false);

#ifdef NOTAGROOT
				if ( not(fast_rep_check) ) {
					std::cout << "[assert] BNE : New PCC should always be representable" << std::endl;
					raise_trap(EXC_INSTR_ACCESS_FAULT, instr.data()); //Non-CHERI exception for ease 
				}
#else
				assert(fast_rep_check && "New PCC should always be representable");
#endif
				pc = last_pc + instr.B_imm();
				trap_check_pc_alignment();

#ifdef TESTRIG
				rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
		}
			break;

		case Opcode::BLT: {
			if (regs[instr.rs1()] < regs[instr.rs2()]) {
				Capability cs;
				cs.addr = last_pc;
				cs.caps = cpc;
				cs.tag = pc_tag;
				int64_t pc_offset = (int64_t)(instr.B_imm());
				bool fast_rep_check = set_offset(cs, pc_offset, false);

#ifdef NOTAGROOT
				if ( not(fast_rep_check) ) {
					std::cout << "[assert] BLT : New PCC should always be representable" << std::endl;
					raise_trap(EXC_INSTR_ACCESS_FAULT, instr.data()); //Non-CHERI exception for ease 
				}
#else
				assert(fast_rep_check && "New PCC should always be representable");
#endif
				pc = last_pc + instr.B_imm();
				trap_check_pc_alignment();

#ifdef TESTRIG
				rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif
			}
		
#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
		}
			break;

		case Opcode::BGE: {
			if (regs[instr.rs1()] >= regs[instr.rs2()]) {
				Capability cs;
				cs.addr = last_pc;
				cs.caps = cpc;
				cs.tag = pc_tag;
				int64_t pc_offset = (int64_t)(instr.B_imm());
				bool fast_rep_check = set_offset(cs, pc_offset, false);

#ifdef NOTAGROOT
				if ( not(fast_rep_check) ) {
					std::cout << "[assert] BGE : New PCC should always be representable" << std::endl;
					raise_trap(EXC_INSTR_ACCESS_FAULT, instr.data()); //Non-CHERI exception for ease 
				}
#else
				assert(fast_rep_check && "New PCC should always be representable");
#endif
				pc = last_pc + instr.B_imm();
				trap_check_pc_alignment();

#ifdef TESTRIG
				rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
		}
			break;

		case Opcode::BLTU: {
			if ((uint32_t)regs[instr.rs1()] < (uint32_t)regs[instr.rs2()]) {
				Capability cs;
				cs.addr = last_pc;
				cs.caps = cpc;
				cs.tag = pc_tag;
				int64_t pc_offset = (int64_t)(instr.B_imm());
				bool fast_rep_check = set_offset(cs, pc_offset, false);

#ifdef NOTAGROOT
				if ( not(fast_rep_check) ) {
					std::cout << "[assert] BLTU : New PCC should always be representable" << std::endl;
					raise_trap(EXC_INSTR_ACCESS_FAULT, instr.data()); //Non-CHERI exception for ease 
				}
#else
				assert(fast_rep_check && "New PCC should always be representable");
#endif
				pc = last_pc + instr.B_imm();
				trap_check_pc_alignment();

#ifdef TESTRIG
				rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
		}
			break;

		case Opcode::BGEU: {
			if ((uint32_t)regs[instr.rs1()] >= (uint32_t)regs[instr.rs2()]) {
				Capability cs;
				cs.addr = last_pc;
				cs.caps = cpc;
				cs.tag = pc_tag;
				int64_t pc_offset = (int64_t)(instr.B_imm());
				bool fast_rep_check = set_offset(cs, pc_offset, false);

#ifdef NOTAGROOT
				if ( not(fast_rep_check) ) {
					std::cout << "[assert] BGEU : New PCC should always be representable" << std::endl;
					raise_trap(EXC_INSTR_ACCESS_FAULT, instr.data()); //Non-CHERI exception for ease 
				}
#else
				assert(fast_rep_check && "New PCC should always be representable");
#endif
				pc = last_pc + instr.B_imm();
				trap_check_pc_alignment();

#ifdef TESTRIG
				rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rs2_addr = static_cast<uint8_t>(instr.rs2());
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs1()]));
			rvfi_dii_output.rvfi_dii_rs2_data = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rs2()]));
#endif
		}
			break;

		case Opcode::FENCE:
		case Opcode::FENCE_I: {
			// not using out of order execution so can be ignored
		} break;

		case Opcode::ECALL: {
#ifdef TESTRIG
			sys = nullptr;
#endif
			if (sys) {
				sys->execute_syscall(this);
			} else {
				switch (prv) {
					case MachineMode:
						raise_trap(EXC_ECALL_M_MODE, last_pc);
						break;
					case SupervisorMode:
						raise_trap(EXC_ECALL_S_MODE, last_pc);
						break;
					case UserMode:
						raise_trap(EXC_ECALL_U_MODE, last_pc);
						break;
					default:
						throw std::runtime_error("unknown privilege level " + std::to_string(prv));
				}
			}
		} break;

		case Opcode::EBREAK: {
#if defined(TESTRIG)
			raise_trap(EXC_BREAKPOINT, last_pc);
#else
			// TODO: also raise trap and let the SW deal with it?
			status = CoreExecStatus::HitBreakpoint;
#endif
		} break;

		case Opcode::CSRRW: {
			auto addr = instr.csr();
			if (is_invalid_csr_access(addr, true)) {
                RAISE_ILLEGAL_INSTRUCTION();
			} else {
				auto rd = instr.rd();
				auto rs1_val = regs[instr.rs1()];
				if (rd != RegFile::zero) {
					regs[instr.rd()] = get_csr_value(addr);
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
					/*spandan--------------------*/
					//For RISC-V instructions, target register should not
					//retain Capability properties 
					regs.cheri[instr.rd()] = NULL_CAP;
					regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
					/*spandan--------------------*/
// }
				}
				set_csr_value(addr, rs1_val);
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint32_t>(regs[instr.rs1()]);
#endif

		} break;

		case Opcode::CSRRS: {
			auto addr = instr.csr();
			auto rs1 = instr.rs1();
			auto write = rs1 != RegFile::zero;
			if (is_invalid_csr_access(addr, write)) {
                RAISE_ILLEGAL_INSTRUCTION();
			} else {
				auto rd = instr.rd();
				auto rs1_val = regs[rs1];
				auto csr_val = get_csr_value(addr);
				if (rd != RegFile::zero) {
					regs[rd] = csr_val;
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
					/*spandan--------------------*/
					//For RISC-V instructions, target register should not
					//retain Capability properties 
					regs.cheri[instr.rd()] = NULL_CAP;
					regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
					/*spandan--------------------*/
// }
				}

				if (write)
					set_csr_value(addr, csr_val | rs1_val);
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint32_t>(regs[instr.rs1()]);
#endif

		} break;

		case Opcode::CSRRC: {
			auto addr = instr.csr();
			auto rs1 = instr.rs1();
			auto write = rs1 != RegFile::zero;
			if (is_invalid_csr_access(addr, write)) {
                RAISE_ILLEGAL_INSTRUCTION();
			} else {
				auto rd = instr.rd();
				auto rs1_val = regs[rs1];
				auto csr_val = get_csr_value(addr);
				if (rd != RegFile::zero) {
					regs[rd] = csr_val;
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
					/*spandan--------------------*/
					//For RISC-V instructions, target register should not
					//retain Capability properties 
					regs.cheri[instr.rd()] = NULL_CAP;
					regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
					/*spandan--------------------*/
// }
				}
				if (write)
					set_csr_value(addr, csr_val & ~rs1_val);
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint32_t>(regs[instr.rs1()]);
#endif

		} break;

		case Opcode::CSRRWI: {
			auto addr = instr.csr();
			if (is_invalid_csr_access(addr, true)) {
                RAISE_ILLEGAL_INSTRUCTION();
			} else {
				auto rd = instr.rd();
				if (rd != RegFile::zero) {
					regs[rd] = get_csr_value(addr);
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
					/*spandan--------------------*/
					//For RISC-V instructions, target register should not
					//retain Capability properties 
					regs.cheri[instr.rd()] = NULL_CAP;
					regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
					/*spandan--------------------*/
// }
				}
				set_csr_value(addr, instr.zimm());
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

		case Opcode::CSRRSI: {
			auto addr = instr.csr();
			auto zimm = instr.zimm();
			auto write = zimm != 0;
			if (is_invalid_csr_access(addr, write)) {
                RAISE_ILLEGAL_INSTRUCTION();
			} else {
				auto csr_val = get_csr_value(addr);
				auto rd = instr.rd();
				if (rd != RegFile::zero) {
					regs[rd] = csr_val;
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
					/*spandan--------------------*/
					//For RISC-V instructions, target register should not
					//retain Capability properties 
					regs.cheri[instr.rd()] = NULL_CAP;
					regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
					/*spandan--------------------*/
// }
				}
				if (write)
					set_csr_value(addr, csr_val | zimm);
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

		case Opcode::CSRRCI: {
			auto addr = instr.csr();
			auto zimm = instr.zimm();
			auto write = zimm != 0;
			if (is_invalid_csr_access(addr, write)) {
                RAISE_ILLEGAL_INSTRUCTION();
			} else {
				auto csr_val = get_csr_value(addr);
				auto rd = instr.rd();
				if (rd != RegFile::zero) {
					regs[rd] = csr_val;
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
					/*spandan--------------------*/
					//For RISC-V instructions, target register should not
					//retain Capability properties 
					regs.cheri[instr.rd()] = NULL_CAP;
					regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
					/*spandan--------------------*/
// }
				}
				if (write)
					set_csr_value(addr, csr_val & ~zimm);
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
#endif

		} break;

/*spandan------------------------------------------------*/
		case Opcode::CSPECIALRW: {
			REQUIRE_ISA(CHERI_ISA_EXT);
			uint8_t scr = instr.rs2();
			uint8_t cs1 = instr.rs1();
			uint32_t pcc_perms = BIT_SLICE( cpc, CAP_PERM_TOP, CAP_PERM_BOT );

			bool specialExists = scr_map_call(scr, 0);
			bool ro = scr_map_call(scr, 1);
			PrivilegeLevel scr_prv = scr_map_call(scr, 2);
			bool needASR = scr_map_call(scr, 3);

			if ( not(specialExists) || (ro && (cs1 != 0)) || (scr_prv > prv) ) {   
				RAISE_ILLEGAL_INSTRUCTION();
			}
			else if (needASR && (BIT_SINGLE(pcc_perms, PERMIT_ACC_SYS_REGS) == 0)) {  
				uint32_t xtval_reg = replace_bits( (csrs.mtval.reg), CAP_ID_BOT, CAP_ID_TOP, (scr+32) );
				xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_ASR_VIOLATE);
				raise_trap(EXC_CHERI, xtval_reg);
			}
			else {
				Capability cd = get_scr(scr);

				if (cs1 != 0) {
					Capability cs;
					cs.addr = regs[instr.rs1()];
					cs.caps = regs.cheri[instr.rs1()];
					cs.tag = (BIT_SINGLE( (regs.tags), (instr.rs1()) ) > 0);
					bool success = set_scr(scr, cs);

					if (trace) {
						if (success) {
							std::cout << regIdent.at( (scr + 32) ) << " <- " << std::hex << cs.addr <<std::endl;
							std::cout << regIdent.at( (scr + 32) ) << "CapBits" << " <- " << std::hex << cs.caps <<std::endl;
							std::cout << regIdent.at( (scr + 32) ) << "Tag" << " <- " << std::hex << cs.tag <<std::endl;
						}
						else {
							std::cout << regIdent.at( (scr + 32) ) << " <- " << std::hex << cd.addr <<std::endl;
							std::cout << regIdent.at( (scr + 32) ) << "CapBits" << " <- " << std::hex << cd.caps <<std::endl;
							std::cout << regIdent.at( (scr + 32) ) << "Tag" << " <- " << std::hex << cd.tag <<std::endl;
						}
					}
				}

				regs[instr.rd()] = cd.addr;
				regs.cheri[instr.rd()] = cd.caps;
				if (cd.tag) regs.tags = SET_BIT( (regs.tags), (instr.rd()) );
				else regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
				
			}

#ifdef TESTRIG
			rvfi_dii_output.rvfi_dii_rd_addr = static_cast<uint8_t>(instr.rd());
			rvfi_dii_output.rvfi_dii_rs1_addr = static_cast<uint8_t>(instr.rs1());
			rvfi_dii_output.rvfi_dii_rd_wdata = static_cast<uint64_t>(static_cast<uint32_t>(regs[instr.rd()]));
			rvfi_dii_output.rvfi_dii_rs1_data = static_cast<uint32_t>(regs[instr.rs1()]);
#endif

		} break;
/*spandan------------------------------------------------*/

		case Opcode::MUL: {
            REQUIRE_ISA(M_ISA_EXT);
			int64_t ans = (int64_t)regs[instr.rs1()] * (int64_t)regs[instr.rs2()];
			regs[instr.rd()] = ans & 0xFFFFFFFF;
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }

		} break;

		case Opcode::MULH: {
            REQUIRE_ISA(M_ISA_EXT);
			int64_t ans = (int64_t)regs[instr.rs1()] * (int64_t)regs[instr.rs2()];
			regs[instr.rd()] = (ans & 0xFFFFFFFF00000000) >> 32;
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }

		} break;

		case Opcode::MULHU: {
            REQUIRE_ISA(M_ISA_EXT);
			int64_t ans = ((uint64_t)(uint32_t)regs[instr.rs1()]) * (uint64_t)((uint32_t)regs[instr.rs2()]);
			regs[instr.rd()] = (ans & 0xFFFFFFFF00000000) >> 32;
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }

		} break;

		case Opcode::MULHSU: {
            REQUIRE_ISA(M_ISA_EXT);
			int64_t ans = (int64_t)regs[instr.rs1()] * (uint64_t)((uint32_t)regs[instr.rs2()]);
			regs[instr.rd()] = (ans & 0xFFFFFFFF00000000) >> 32;
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }

		} break;

		case Opcode::DIV: {
            REQUIRE_ISA(M_ISA_EXT);
			auto a = regs[instr.rs1()];
			auto b = regs[instr.rs2()];
			if (b == 0) {
				regs[instr.rd()] = -1;
			} else if (a == REG_MIN && b == -1) {
				regs[instr.rd()] = a;
			} else {
				regs[instr.rd()] = a / b;
			}
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }

		} break;

		case Opcode::DIVU: {
            REQUIRE_ISA(M_ISA_EXT);
			auto a = regs[instr.rs1()];
			auto b = regs[instr.rs2()];
			if (b == 0) {
				regs[instr.rd()] = -1;
			} else {
				regs[instr.rd()] = (uint32_t)a / (uint32_t)b;
			}
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }

		} break;

		case Opcode::REM: {
            REQUIRE_ISA(M_ISA_EXT);
			auto a = regs[instr.rs1()];
			auto b = regs[instr.rs2()];
			if (b == 0) {
				regs[instr.rd()] = a;
			} else if (a == REG_MIN && b == -1) {
				regs[instr.rd()] = 0;
			} else {
				regs[instr.rd()] = a % b;
			}
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }

		} break;

		case Opcode::REMU: {
            REQUIRE_ISA(M_ISA_EXT);
			auto a = regs[instr.rs1()];
			auto b = regs[instr.rs2()];
			if (b == 0) {
				regs[instr.rd()] = a;
			} else {
				regs[instr.rd()] = (uint32_t)a % (uint32_t)b;
			}
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }

		} break;

		case Opcode::LR_W: {
            REQUIRE_ISA(A_ISA_EXT);
			uint32_t addr = regs[instr.rs1()];
			trap_check_addr_alignment<4, true>(addr);
			regs[instr.rd()] = mem->atomic_load_reserved_word(addr);
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }
			if (lr_sc_counter == 0)
			    lr_sc_counter = 17;  // this instruction + 16 additional ones, (an over-approximation) to cover the RISC-V forward progress property

		} break;

		case Opcode::SC_W: {
            REQUIRE_ISA(A_ISA_EXT);
			uint32_t addr = regs[instr.rs1()];
			trap_check_addr_alignment<4, false>(addr);
			uint32_t val = regs[instr.rs2()];
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }
			regs[instr.rd()] = 1;  // failure by default (in case a trap is thrown)
			regs[instr.rd()] = mem->atomic_store_conditional_word(addr, val) ? 0 : 1;  // overwrite result (in case no trap is thrown)
			lr_sc_counter = 0;

		} break;

		case Opcode::AMOSWAP_W: {
            REQUIRE_ISA(A_ISA_EXT);
			execute_amo(instr, [](int32_t a, int32_t b) {
				(void)a;
				return b;
			});
		} break;

		case Opcode::AMOADD_W: {
            REQUIRE_ISA(A_ISA_EXT);
			execute_amo(instr, [](int32_t a, int32_t b) { return a + b; });
		} break;

		case Opcode::AMOXOR_W: {
            REQUIRE_ISA(A_ISA_EXT);
			execute_amo(instr, [](int32_t a, int32_t b) { return a ^ b; });
		} break;

		case Opcode::AMOAND_W: {
            REQUIRE_ISA(A_ISA_EXT);
			execute_amo(instr, [](int32_t a, int32_t b) { return a & b; });
		} break;

		case Opcode::AMOOR_W: {
            REQUIRE_ISA(A_ISA_EXT);
			execute_amo(instr, [](int32_t a, int32_t b) { return a | b; });
		} break;

		case Opcode::AMOMIN_W: {
            REQUIRE_ISA(A_ISA_EXT);
			execute_amo(instr, [](int32_t a, int32_t b) { return std::min(a, b); });
		} break;

		case Opcode::AMOMINU_W: {
            REQUIRE_ISA(A_ISA_EXT);
			execute_amo(instr, [](int32_t a, int32_t b) { return std::min((uint32_t)a, (uint32_t)b); });
		} break;

		case Opcode::AMOMAX_W: {
            REQUIRE_ISA(A_ISA_EXT);
			execute_amo(instr, [](int32_t a, int32_t b) { return std::max(a, b); });
		} break;

		case Opcode::AMOMAXU_W: {
            REQUIRE_ISA(A_ISA_EXT);
			execute_amo(instr, [](int32_t a, int32_t b) { return std::max((uint32_t)a, (uint32_t)b); });
		} break;

			// RV32F Extension

		case Opcode::FLW: {
            REQUIRE_ISA(F_ISA_EXT);
			uint32_t addr = regs[instr.rs1()] + instr.I_imm();
			trap_check_addr_alignment<4, true>(addr);
			fp_regs.write(RD, float32_t{(uint32_t)mem->load_word(addr)});
		} break;

		case Opcode::FSW: {
            REQUIRE_ISA(F_ISA_EXT);
			uint32_t addr = regs[instr.rs1()] + instr.S_imm();
			trap_check_addr_alignment<4, false>(addr);
            mem->store_word(addr, fp_regs.u32(RS2));
		} break;

		case Opcode::FADD_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			fp_regs.write(RD, f32_add(fp_regs.f32(RS1), fp_regs.f32(RS2)));
			fp_finish_instr();
		} break;

		case Opcode::FSUB_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			fp_regs.write(RD, f32_sub(fp_regs.f32(RS1), fp_regs.f32(RS2)));
			fp_finish_instr();
		} break;

		case Opcode::FMUL_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			fp_regs.write(RD, f32_mul(fp_regs.f32(RS1), fp_regs.f32(RS2)));
			fp_finish_instr();
		} break;

		case Opcode::FDIV_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			fp_regs.write(RD, f32_div(fp_regs.f32(RS1), fp_regs.f32(RS2)));
			fp_finish_instr();
		} break;

		case Opcode::FSQRT_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			fp_regs.write(RD, f32_sqrt(fp_regs.f32(RS1)));
			fp_finish_instr();
		} break;

		case Opcode::FMIN_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();

			bool rs1_smaller = f32_lt_quiet(fp_regs.f32(RS1), fp_regs.f32(RS2)) ||
			                   (f32_eq(fp_regs.f32(RS1), fp_regs.f32(RS2)) && f32_isNegative(fp_regs.f32(RS1)));

			if (f32_isNaN(fp_regs.f32(RS1)) && f32_isNaN(fp_regs.f32(RS2))) {
				fp_regs.write(RD, f32_defaultNaN);
			} else {
				if (rs1_smaller)
					fp_regs.write(RD, fp_regs.f32(RS1));
				else
					fp_regs.write(RD, fp_regs.f32(RS2));
			}

			fp_finish_instr();
		} break;

		case Opcode::FMAX_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();

			bool rs1_greater = f32_lt_quiet(fp_regs.f32(RS2), fp_regs.f32(RS1)) ||
			                   (f32_eq(fp_regs.f32(RS2), fp_regs.f32(RS1)) && f32_isNegative(fp_regs.f32(RS2)));

			if (f32_isNaN(fp_regs.f32(RS1)) && f32_isNaN(fp_regs.f32(RS2))) {
				fp_regs.write(RD, f32_defaultNaN);
			} else {
				if (rs1_greater)
					fp_regs.write(RD, fp_regs.f32(RS1));
				else
					fp_regs.write(RD, fp_regs.f32(RS2));
			}

			fp_finish_instr();
		} break;

		case Opcode::FMADD_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			fp_regs.write(RD, f32_mulAdd(fp_regs.f32(RS1), fp_regs.f32(RS2), fp_regs.f32(RS3)));
			fp_finish_instr();
		} break;

		case Opcode::FMSUB_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			fp_regs.write(RD, f32_mulAdd(fp_regs.f32(RS1), fp_regs.f32(RS2), f32_neg(fp_regs.f32(RS3))));
			fp_finish_instr();
		} break;

		case Opcode::FNMADD_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			fp_regs.write(RD, f32_mulAdd(f32_neg(fp_regs.f32(RS1)), fp_regs.f32(RS2), f32_neg(fp_regs.f32(RS3))));
			fp_finish_instr();
		} break;

		case Opcode::FNMSUB_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			fp_regs.write(RD, f32_mulAdd(f32_neg(fp_regs.f32(RS1)), fp_regs.f32(RS2), fp_regs.f32(RS3)));
			fp_finish_instr();
		} break;

		case Opcode::FCVT_W_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			regs[RD] = f32_to_i32(fp_regs.f32(RS1), softfloat_roundingMode, true);
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }
			fp_finish_instr();
		} break;

		case Opcode::FCVT_WU_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			regs[RD] = f32_to_ui32(fp_regs.f32(RS1), softfloat_roundingMode, true);
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }
			fp_finish_instr();
		} break;

		case Opcode::FCVT_S_W: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			fp_regs.write(RD, i32_to_f32(regs[RS1]));
			fp_finish_instr();
		} break;

		case Opcode::FCVT_S_WU: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			fp_regs.write(RD, ui32_to_f32(regs[RS1]));
			fp_finish_instr();
		} break;

		case Opcode::FSGNJ_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			auto f1 = fp_regs.f32(RS1);
			auto f2 = fp_regs.f32(RS2);
			fp_regs.write(RD, float32_t{(f1.v & ~F32_SIGN_BIT) | (f2.v & F32_SIGN_BIT)});
			fp_set_dirty();
		} break;

		case Opcode::FSGNJN_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			auto f1 = fp_regs.f32(RS1);
			auto f2 = fp_regs.f32(RS2);
			fp_regs.write(RD, float32_t{(f1.v & ~F32_SIGN_BIT) | (~f2.v & F32_SIGN_BIT)});
			fp_set_dirty();
		} break;

		case Opcode::FSGNJX_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			auto f1 = fp_regs.f32(RS1);
			auto f2 = fp_regs.f32(RS2);
			fp_regs.write(RD, float32_t{f1.v ^ (f2.v & F32_SIGN_BIT)});
			fp_set_dirty();
		} break;

		case Opcode::FMV_W_X: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_regs.write(RD, float32_t{(uint32_t)regs[RS1]});
			fp_set_dirty();
		} break;

		case Opcode::FMV_X_W: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			regs[RD] = fp_regs.u32(RS1);
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }

		} break;

		case Opcode::FEQ_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			regs[RD] = f32_eq(fp_regs.f32(RS1), fp_regs.f32(RS2));
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }
			fp_update_exception_flags();

		} break;

		case Opcode::FLT_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			regs[RD] = f32_lt(fp_regs.f32(RS1), fp_regs.f32(RS2));
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }
			fp_update_exception_flags();

		} break;

		case Opcode::FLE_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			regs[RD] = f32_le(fp_regs.f32(RS1), fp_regs.f32(RS2));
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }
			fp_update_exception_flags();

		} break;

		case Opcode::FCLASS_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			regs[RD] = f32_classify(fp_regs.f32(RS1));
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }

		} break;

			// RV32D Extension

        case Opcode::FLD: {
            REQUIRE_ISA(D_ISA_EXT);
            uint32_t addr = regs[instr.rs1()] + instr.I_imm();
            trap_check_addr_alignment<8, true>(addr);
            fp_regs.write(RD, float64_t{(uint64_t)mem->load_double(addr)});
        } break;

        case Opcode::FSD: {
            REQUIRE_ISA(D_ISA_EXT);
            uint32_t addr = regs[instr.rs1()] + instr.S_imm();
            trap_check_addr_alignment<8, false>(addr);
            mem->store_double(addr, fp_regs.f64(RS2).v);
        } break;

        case Opcode::FADD_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, f64_add(fp_regs.f64(RS1), fp_regs.f64(RS2)));
            fp_finish_instr();
        } break;

        case Opcode::FSUB_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, f64_sub(fp_regs.f64(RS1), fp_regs.f64(RS2)));
            fp_finish_instr();
        } break;

        case Opcode::FMUL_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, f64_mul(fp_regs.f64(RS1), fp_regs.f64(RS2)));
            fp_finish_instr();
        } break;

        case Opcode::FDIV_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, f64_div(fp_regs.f64(RS1), fp_regs.f64(RS2)));
            fp_finish_instr();
        } break;

        case Opcode::FSQRT_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, f64_sqrt(fp_regs.f64(RS1)));
            fp_finish_instr();
        } break;

        case Opcode::FMIN_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();

            bool rs1_smaller = f64_lt_quiet(fp_regs.f64(RS1), fp_regs.f64(RS2)) ||
                               (f64_eq(fp_regs.f64(RS1), fp_regs.f64(RS2)) && f64_isNegative(fp_regs.f64(RS1)));

            if (f64_isNaN(fp_regs.f64(RS1)) && f64_isNaN(fp_regs.f64(RS2))) {
                fp_regs.write(RD, f64_defaultNaN);
            } else {
                if (rs1_smaller)
                    fp_regs.write(RD, fp_regs.f64(RS1));
                else
                    fp_regs.write(RD, fp_regs.f64(RS2));
            }

            fp_finish_instr();
        } break;

        case Opcode::FMAX_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();

            bool rs1_greater = f64_lt_quiet(fp_regs.f64(RS2), fp_regs.f64(RS1)) ||
                               (f64_eq(fp_regs.f64(RS2), fp_regs.f64(RS1)) && f64_isNegative(fp_regs.f64(RS2)));

            if (f64_isNaN(fp_regs.f64(RS1)) && f64_isNaN(fp_regs.f64(RS2))) {
                fp_regs.write(RD, f64_defaultNaN);
            } else {
                if (rs1_greater)
                    fp_regs.write(RD, fp_regs.f64(RS1));
                else
                    fp_regs.write(RD, fp_regs.f64(RS2));
            }

            fp_finish_instr();
        } break;

        case Opcode::FMADD_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, f64_mulAdd(fp_regs.f64(RS1), fp_regs.f64(RS2), fp_regs.f64(RS3)));
            fp_finish_instr();
        } break;

        case Opcode::FMSUB_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, f64_mulAdd(fp_regs.f64(RS1), fp_regs.f64(RS2), f64_neg(fp_regs.f64(RS3))));
            fp_finish_instr();
        } break;

        case Opcode::FNMADD_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, f64_mulAdd(f64_neg(fp_regs.f64(RS1)), fp_regs.f64(RS2), f64_neg(fp_regs.f64(RS3))));
            fp_finish_instr();
        } break;

        case Opcode::FNMSUB_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, f64_mulAdd(f64_neg(fp_regs.f64(RS1)), fp_regs.f64(RS2), fp_regs.f64(RS3)));
            fp_finish_instr();
        } break;

        case Opcode::FSGNJ_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            auto f1 = fp_regs.f64(RS1);
            auto f2 = fp_regs.f64(RS2);
            fp_regs.write(RD, float64_t{(f1.v & ~F64_SIGN_BIT) | (f2.v & F64_SIGN_BIT)});
            fp_set_dirty();
        } break;

        case Opcode::FSGNJN_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            auto f1 = fp_regs.f64(RS1);
            auto f2 = fp_regs.f64(RS2);
            fp_regs.write(RD, float64_t{(f1.v & ~F64_SIGN_BIT) | (~f2.v & F64_SIGN_BIT)});
            fp_set_dirty();
        } break;

        case Opcode::FSGNJX_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            auto f1 = fp_regs.f64(RS1);
            auto f2 = fp_regs.f64(RS2);
            fp_regs.write(RD, float64_t{f1.v ^ (f2.v & F64_SIGN_BIT)});
            fp_set_dirty();
        } break;

        case Opcode::FCVT_S_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, f64_to_f32(fp_regs.f64(RS1)));
            fp_finish_instr();
        } break;

        case Opcode::FCVT_D_S: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, f32_to_f64(fp_regs.f32(RS1)));
            fp_finish_instr();
        } break;

        case Opcode::FEQ_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            regs[RD] = f64_eq(fp_regs.f64(RS1), fp_regs.f64(RS2));
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }
            fp_update_exception_flags();

        } break;

        case Opcode::FLT_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            regs[RD] = f64_lt(fp_regs.f64(RS1), fp_regs.f64(RS2));
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }
            fp_update_exception_flags();

        } break;

        case Opcode::FLE_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            regs[RD] = f64_le(fp_regs.f64(RS1), fp_regs.f64(RS2));
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }
            fp_update_exception_flags();

        } break;

        case Opcode::FCLASS_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            regs[RD] = (int64_t)f64_classify(fp_regs.f64(RS1));
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }

        } break;

        case Opcode::FCVT_W_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            regs[RD] = f64_to_i32(fp_regs.f64(RS1), softfloat_roundingMode, true);
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }
            fp_finish_instr();

        } break;

        case Opcode::FCVT_WU_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            regs[RD] = (int32_t)f64_to_ui32(fp_regs.f64(RS1), softfloat_roundingMode, true);
// if (BIT_SINGLE(cpc, CAP_FLAG)) {
			/*spandan--------------------*/
			//For RISC-V instructions, target register should not
			//retain Capability properties 
			regs.cheri[instr.rd()] = NULL_CAP;
			regs.tags = UNSET_BIT( (regs.tags), (instr.rd()) );
			/*spandan--------------------*/
// }
            fp_finish_instr();

        } break;

        case Opcode::FCVT_D_W: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, i32_to_f64((int32_t)regs[RS1]));
            fp_finish_instr();
        } break;

        case Opcode::FCVT_D_WU: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            fp_regs.write(RD, ui32_to_f64((int32_t)regs[RS1]));
            fp_finish_instr();
        } break;

        // privileged instructions

        case Opcode::WFI:
            // NOTE: only a hint, can be implemented as NOP
            // std::cout << "[sim:wfi] CSR mstatus.mie " << csrs.mstatus->mie << std::endl;
            release_lr_sc_reservation();

            if (s_mode() && csrs.mstatus.fields.tw)
                raise_trap(EXC_ILLEGAL_INSTR, instr.data());

            if (u_mode() && csrs.misa.has_supervisor_mode_extension())
                raise_trap(EXC_ILLEGAL_INSTR, instr.data());

            if (!ignore_wfi && !has_local_pending_enabled_interrupts())
                sc_core::wait(wfi_event);
            break;

        case Opcode::SFENCE_VMA:
            if (s_mode() && csrs.mstatus.fields.tvm)
                raise_trap(EXC_ILLEGAL_INSTR, instr.data());
            mem->flush_tlb();
            break;

        case Opcode::URET:
#ifdef TESTRIG
			raise_trap(EXC_ILLEGAL_INSTR, instr.data());
#endif
            if (!csrs.misa.has_user_mode_extension())
                raise_trap(EXC_ILLEGAL_INSTR, instr.data());
            return_from_trap_handler(UserMode);
            break;

        case Opcode::SRET:
#ifdef TESTRIG
			if(prv < SupervisorMode) raise_trap(EXC_ILLEGAL_INSTR, instr.data());
#endif
            if (!csrs.misa.has_supervisor_mode_extension() || (s_mode() && csrs.mstatus.fields.tsr))
                raise_trap(EXC_ILLEGAL_INSTR, instr.data());
            return_from_trap_handler(SupervisorMode);
            break;

        case Opcode::MRET:
#ifdef TESTRIG
			if(prv < MachineMode) raise_trap(EXC_ILLEGAL_INSTR, instr.data());
#endif
            return_from_trap_handler(MachineMode);
            break;

            // instructions accepted by decoder but not by this RV32IMACF ISS -> do normal trap
            // RV64I
        case Opcode::LWU:
        //case Opcode::LD: //spandan: Need this for [C]LC
        //case Opcode::SD: //spandan: Need this for [C]SC
        case Opcode::ADDIW:
        case Opcode::SLLIW:
        case Opcode::SRLIW:
        case Opcode::SRAIW:
        case Opcode::ADDW:
        case Opcode::SUBW:
        case Opcode::SLLW:
        case Opcode::SRLW:
        case Opcode::SRAW:
            // RV64M
        case Opcode::MULW:
        case Opcode::DIVW:
        case Opcode::DIVUW:
        case Opcode::REMW:
        case Opcode::REMUW:
            // RV64A
        case Opcode::LR_D:
        case Opcode::SC_D:
        case Opcode::AMOSWAP_D:
        case Opcode::AMOADD_D:
        case Opcode::AMOXOR_D:
        case Opcode::AMOAND_D:
        case Opcode::AMOOR_D:
        case Opcode::AMOMIN_D:
        case Opcode::AMOMAX_D:
        case Opcode::AMOMINU_D:
        case Opcode::AMOMAXU_D:
            // RV64F
        case Opcode::FCVT_L_S:
        case Opcode::FCVT_LU_S:
        case Opcode::FCVT_S_L:
        case Opcode::FCVT_S_LU:
            // RV64D
        case Opcode::FCVT_L_D:
        case Opcode::FCVT_LU_D:
        case Opcode::FMV_X_D:
        case Opcode::FCVT_D_L:
        case Opcode::FCVT_D_LU:
        case Opcode::FMV_D_X:
            RAISE_ILLEGAL_INSTRUCTION();
            break;

        default:
            throw std::runtime_error("unknown opcode");
	}
}

uint64_t ISS::_compute_and_get_current_cycles() {
	assert(cycle_counter % cycle_time == sc_core::SC_ZERO_TIME);
	assert(cycle_counter.value() % cycle_time.value() == 0);

	uint64_t num_cycles = cycle_counter.value() / cycle_time.value();

	return num_cycles;
}


bool ISS::is_invalid_csr_access(uint32_t csr_addr, bool is_write) {
    if (csr_addr == csr::FFLAGS_ADDR || csr_addr == csr::FRM_ADDR || csr_addr == csr::FCSR_ADDR) {
        REQUIRE_ISA(F_ISA_EXT);
    }
    PrivilegeLevel csr_prv = (0x300 & csr_addr) >> 8;
    bool csr_readonly = ((0xC00 & csr_addr) >> 10) == 3;
    bool s_invalid = (csr_prv == SupervisorMode) && !csrs.misa.has_supervisor_mode_extension();
    bool u_invalid = (csr_prv == UserMode) && !csrs.misa.has_user_mode_extension();
    return (is_write && csr_readonly) || (prv < csr_prv) || s_invalid || u_invalid;
}


void ISS::validate_csr_counter_read_access_rights(uint32_t addr) {
	// match against counter CSR addresses, see RISC-V privileged spec for the address definitions
	if ((addr >= 0xC00 && addr <= 0xC1F) || (addr >= 0xC80 && addr <= 0xC9F)) {
		auto cnt = addr & 0x1F;  // 32 counter in total, naturally aligned with the mcounteren and scounteren CSRs

		if (s_mode() && !csr::is_bitset(csrs.mcounteren, cnt))
			RAISE_ILLEGAL_INSTRUCTION();

		if (u_mode() && (!csr::is_bitset(csrs.mcounteren, cnt) || !csr::is_bitset(csrs.scounteren, cnt)))
			RAISE_ILLEGAL_INSTRUCTION();
	}
}

uint32_t ISS::get_csr_value(uint32_t addr) {
	validate_csr_counter_read_access_rights(addr);

	auto read = [=](auto &x, uint32_t mask) { return x.reg & mask; };

	using namespace csr;

	switch (addr) {
		case TIME_ADDR:
		case MTIME_ADDR: {
			uint64_t mtime = clint->update_and_get_mtime();
			csrs.time.reg = mtime;
			return csrs.time.words.low;
		}

		case TIMEH_ADDR:
		case MTIMEH_ADDR: {
			uint64_t mtime = clint->update_and_get_mtime();
			csrs.time.reg = mtime;
			return csrs.time.words.high;
		}

		case MCYCLE_ADDR:
			csrs.cycle.reg = _compute_and_get_current_cycles();
			return csrs.cycle.words.low;

		case MCYCLEH_ADDR:
			csrs.cycle.reg = _compute_and_get_current_cycles();
			return csrs.cycle.words.high;

		case MINSTRET_ADDR:
			return csrs.instret.words.low;

		case MINSTRETH_ADDR:
			return csrs.instret.words.high;

		SWITCH_CASE_MATCH_ANY_HPMCOUNTER_RV32:  // not implemented
			return 0;

		case MSTATUS_ADDR:
			return read(csrs.mstatus, MSTATUS_MASK);
		case SSTATUS_ADDR:
			return read(csrs.mstatus, SSTATUS_MASK);
		case USTATUS_ADDR:
			return read(csrs.mstatus, USTATUS_MASK);

		case MIP_ADDR:
			return read(csrs.mip, MIP_READ_MASK);
		case SIP_ADDR:
			return read(csrs.mip, SIP_MASK);
		case UIP_ADDR:
			return read(csrs.mip, UIP_MASK);

		case MIE_ADDR:
			return read(csrs.mie, MIE_MASK);
		case SIE_ADDR:
			return read(csrs.mie, SIE_MASK);
		case UIE_ADDR:
			return read(csrs.mie, UIE_MASK);

		case SATP_ADDR:
			if (csrs.mstatus.fields.tvm)
				RAISE_ILLEGAL_INSTRUCTION();
			break;

		case FCSR_ADDR:
			return read(csrs.fcsr, FCSR_MASK);

		case FFLAGS_ADDR:
			return csrs.fcsr.fields.fflags;

		case FRM_ADDR:
			return csrs.fcsr.fields.frm;

        // debug CSRs not supported, thus hardwired
        case TSELECT_ADDR:
            return 1; // if a zero write by SW is preserved, then debug mode is supported (thus hardwire to non-zero)
        case TDATA1_ADDR:
        case TDATA2_ADDR:
        case TDATA3_ADDR:
        case DCSR_ADDR:
        case DPC_ADDR:
        case DSCRATCH0_ADDR:
        case DSCRATCH1_ADDR:
            return 0;
	}

	if (!csrs.is_valid_csr32_addr(addr))
		RAISE_ILLEGAL_INSTRUCTION();

	return csrs.default_read32(addr);
}

void ISS::set_csr_value(uint32_t addr, uint32_t value) {
	auto write = [=](auto &x, uint32_t mask) { x.reg = (x.reg & ~mask) | (value & mask); };

	using namespace csr;

	switch (addr) {
		case MISA_ADDR:                         // currently, read-only, thus cannot be changed at runtime
		SWITCH_CASE_MATCH_ANY_HPMCOUNTER_RV32:  // not implemented
			break;

        case SATP_ADDR: {
            if (csrs.mstatus.fields.tvm)
                RAISE_ILLEGAL_INSTRUCTION();
            write(csrs.satp, SATP_MASK);
            // std::cout << "[iss] satp=" << boost::format("%x") % csrs.satp.reg << std::endl;
        } break;

		case MTVEC_ADDR:
			write(csrs.mtvec, MTVEC_MASK);
			break;
		case STVEC_ADDR:
			write(csrs.stvec, MTVEC_MASK);
			break;
		case UTVEC_ADDR:
			write(csrs.utvec, MTVEC_MASK);
			break;

		case MEPC_ADDR:
			write(csrs.mepc, pc_alignment_mask());
			break;
		case SEPC_ADDR:
			write(csrs.sepc, pc_alignment_mask());
			break;
		case UEPC_ADDR:
			write(csrs.uepc, pc_alignment_mask());
			break;

		case MSTATUS_ADDR:
			write(csrs.mstatus, MSTATUS_MASK);
			break;
		case SSTATUS_ADDR:
			write(csrs.mstatus, SSTATUS_MASK);
			break;
		case USTATUS_ADDR:
			write(csrs.mstatus, USTATUS_MASK);
			break;

		case MIP_ADDR:
			write(csrs.mip, MIP_WRITE_MASK);
			break;
		case SIP_ADDR:
			write(csrs.mip, SIP_MASK);
			break;
		case UIP_ADDR:
			write(csrs.mip, UIP_MASK);
			break;

		case MIE_ADDR:
			write(csrs.mie, MIE_MASK);
			break;
		case SIE_ADDR:
			write(csrs.mie, SIE_MASK);
			break;
		case UIE_ADDR:
			write(csrs.mie, UIE_MASK);
			break;

		case MIDELEG_ADDR:
			write(csrs.mideleg, MIDELEG_MASK);
			break;

		case MEDELEG_ADDR:
			write(csrs.medeleg, MEDELEG_MASK);
			break;

		case SIDELEG_ADDR:
			write(csrs.sideleg, SIDELEG_MASK);
			break;

		case SEDELEG_ADDR:
			write(csrs.sedeleg, SEDELEG_MASK);
			break;

		case MCOUNTEREN_ADDR:
			write(csrs.mcounteren, MCOUNTEREN_MASK);
			break;

		case SCOUNTEREN_ADDR:
			write(csrs.scounteren, MCOUNTEREN_MASK);
			break;

		case MCOUNTINHIBIT_ADDR:
			write(csrs.mcountinhibit, MCOUNTINHIBIT_MASK);
			break;

		case FCSR_ADDR:
			write(csrs.fcsr, FCSR_MASK);
			break;

		case FFLAGS_ADDR:
			csrs.fcsr.fields.fflags = value;
			break;

		case FRM_ADDR:
			csrs.fcsr.fields.frm = value;
			break;

        // debug CSRs not supported, thus hardwired
        case TSELECT_ADDR:
        case TDATA1_ADDR:
        case TDATA2_ADDR:
        case TDATA3_ADDR:
        case DCSR_ADDR:
        case DPC_ADDR:
        case DSCRATCH0_ADDR:
        case DSCRATCH1_ADDR:
            break;

		default:
			if (!csrs.is_valid_csr32_addr(addr))
				RAISE_ILLEGAL_INSTRUCTION();

			csrs.default_write32(addr, value);
	}
}

/*spandan------------------------------------------------------------------*/
void ISS::reset_core() {
	memset(regs.regs, 0, sizeof(regs.regs));
	for (size_t i = 0; i < regs.NUM_REGS; ++i) {
    	regs.cheri[i] = NULL_CAP;
	}
	memset(&(regs.tags), 0, sizeof(uint32_t));


#ifdef TESTRIG
	for (size_t i=1; i<regs.NUM_REGS; ++i) {
		regs.cheri[i] = replace_bits(regs.cheri[i], CAP_PERM_BOT, CAP_PERM_TOP, 0b111111111111);
	}
	regs.tags = ~1u;  // set tag for all registers except x0
#endif


#ifdef TESTRIG
	pc = 0x80000000;  // reset pc to TestRIG default
	prv = MachineMode;  // reset privilege mode to Machine Mode
#else
	pc = 0;  // reset pc to 0 by default
#endif
	cpc = INF_CAP;
	pc_tag = true;
	
	pre_ddc = 0;
	cpre_ddc = INF_CAP;
	pre_ddc_tag = true;

	csrs.reset();

	mem->reset_mem();
}
/*spandan------------------------------------------------------------------*/

/*spandan: set rvfi_dii_interface------------------------------------------*/
/*
void ISS::set_rvfi_dii(std::shared_ptr<rvfi_dii_interface_t> iface) {
    rvfi_dii = std::move(iface);
}
*/
/*spandan------------------------------------------------------------------*/

void ISS::init(instr_memory_if *instr_mem, data_memory_if *data_mem, clint_if *clint, uint32_t entrypoint,
               uint32_t sp) {
	this->instr_mem = instr_mem;
	this->mem = data_mem;
	this->clint = clint;
	regs[RegFile::sp] = sp;
	pc = entrypoint;
}

void ISS::sys_exit() {
	shall_exit = true;
}

unsigned ISS::get_syscall_register_index() {
	if (csrs.misa.has_E_base_isa())
		return RegFile::a5;
	else
		return RegFile::a7;
}


uint64_t ISS::read_register(unsigned idx) {
	return (uint32_t)regs.read(idx);    //NOTE: zero extend
}

void ISS::write_register(unsigned idx, uint64_t value) {
	// Since the value parameter in the function prototype is
	// a uint64_t, signed integer values (e.g. -1) get promoted
	// to values within this range. For example, -1 would be
	// promoted to (2**64)-1. As such, we cannot perform a
	// Boost lexical or numeric cast to uint32_t here as
	// these perform bounds checks. Instead, we perform a C
	// cast without bounds checks.
	regs.write(idx, (uint32_t)value);
}

/*spandan------------------------------------------------------------------*/
//CHERI
uint32_t ISS::get_base(uint32_t addr, uint32_t caps) {
	uint8_t B, E, Amid, R;
	int8_t Bupper;
	int8_t cb;
	uint32_t b;

	if (caps & (1 << CAP_IE)) {
		E = (((caps >> CAP_L7) & 1) << 4) | ((BIT_SLICE(caps, CAP_TE_TOP, CAP_TE_BOT)) << 2) | BIT_RANGE(caps, CAP_BE_TOP, CAP_BE_BOT);
		B = BIT_RANGE(caps, CAP_B_TOP, CAP_B_BOT) | 0;
	}
	else {
		E = 0;
		B = BIT_RANGE(caps, CAP_B_TOP, CAP_BE_BOT);
	}

	assert (E < (E_MAX+1));

	Bupper = BIT_SLICE(B, R_TOP, R_BOT);
	//As E+CAP_B_TOP might exceed 31
	uint64_t Atemp = static_cast<uint64_t>(addr);
	Amid = BIT_SLICE(Atemp, (E+CAP_B_TOP), E);
	R = (static_cast<uint8_t>(Bupper - 1) << R_BOT) | 0;

	if (Amid < R) {
		if (B < R) cb = 0;
		else cb = -1;
	}
	else {
		if (B < R) cb = 1;
		else cb = 0;
	}

	if ( (E+CAP_B_TOP+1) > 31 )
		b = B << E;
	else
		b = ((BIT_SLICE(addr, CAP_TOP, (E+CAP_B_TOP+1)) + cb) << (E+CAP_B_TOP+1)) | (B << E) | 0;

	return b;
}


//top can be a 33 bit value at max, hence represented as 64 bit signed integer
int64_t ISS::get_top(uint32_t addr, uint32_t caps) {
	uint8_t T, B, T_5_2, B_5_2, T_5_0, B_5_0, T_7_6, B_7_6, L_cout, L_msb, E, Amid, R;
	int8_t Bupper;
	int8_t ct, cb;
	int8_t diff_thigh_bhigh;
	int64_t t;
	uint32_t b;

	if (caps & (1 << CAP_IE)) {
		E = (((caps >> CAP_L7) & 1) << 4) | ((BIT_SLICE(caps, CAP_TE_TOP, CAP_TE_BOT)) << 2) | BIT_RANGE(caps, CAP_BE_TOP, CAP_BE_BOT);
		T_5_2 = BIT_SLICE(caps, CAP_T_TOP, CAP_T_BOT);
		B_5_2 = BIT_SLICE(caps, (CAP_B_TOP-2), CAP_B_BOT);
		L_msb = 1;

		if (T_5_2 < B_5_2) L_cout = 1;
		else L_cout = 0;

		B_7_6 = BIT_SLICE(caps, CAP_B_TOP, (CAP_B_TOP-1));
		T_7_6 = B_7_6 + L_cout + L_msb;
		T = (T_7_6 << 6) | (T_5_2 << 2) | 0;
		B = (B_7_6 << 6) | (B_5_2 << 2) | 0; //required for cb calculation
	}
	else {
		E = 0;
		T_5_0 = BIT_SLICE(caps, CAP_T_TOP, CAP_TE_BOT);
		B_5_0 = BIT_RANGE(caps, (CAP_B_TOP-2), CAP_BE_BOT);
		L_msb = ((caps >> CAP_L7) & 0b1);

		if (T_5_0 < B_5_0) L_cout = 1;
		else L_cout = 0;

		B_7_6 = BIT_SLICE(caps, CAP_B_TOP, (CAP_B_TOP-1));
		T_7_6 = B_7_6 + L_cout + L_msb;
		T = (T_7_6 << 6) | T_5_0;
		B = (B_7_6 << 6) | B_5_0; //required for cb calculation
	}

	assert (E < (E_MAX+1));
	assert (L_msb==1 || L_msb==0);
	assert (L_cout==1 || L_cout==0);

	Bupper = BIT_SLICE(caps, R_TOP, R_BOT);
	//As E+CAP_B_TOP might exceed 31
	uint64_t Atemp = static_cast<uint64_t>(addr);
	Amid = BIT_SLICE(Atemp, (E+CAP_B_TOP), E);
	R = (static_cast<uint8_t>(Bupper - 1) << R_BOT) | 0;

	if (Amid < R) {
		if (T < R) ct = 0;
		else ct = -1;

		if (B < R) cb = 0;
		else cb = -1;
	}
	else {
		if (T < R) ct = 1;
		else ct = 0;

		if (B < R) cb = 1;
		else cb = 0;
	}

	if ( (E+CAP_B_TOP+1) > 31 ) {
		t = (static_cast<int64_t>(T)) << E;
		b = (static_cast<uint32_t>(B)) << E;
	}
	else {
		t = ((BIT_SLICE(addr, CAP_TOP, (E+CAP_B_TOP+1)) + ct) << (E+CAP_B_TOP+1)) | (T << E) | 0;
		b = ((BIT_SLICE(addr, CAP_TOP, (E+CAP_B_TOP+1)) + cb) << (E+CAP_B_TOP+1)) | 0;
	}

	//b does not need to be exact, we only need the topmost bit
	diff_thigh_bhigh = static_cast<int8_t>(TOP_TWO(t)) - static_cast<int8_t>(BIT_SINGLE_P1(b, CAP_TOP));
	if ( (E < (E_MAX-1)) && (diff_thigh_bhigh > 1) ) t ^= (1ULL << ADDR_BITS); //invert the 32nd bit of t

	return t;
}


uint32_t ISS::get_len(uint32_t addr, uint32_t caps) {
	int64_t ltemp = get_top(addr, caps) - get_base(addr, caps);

#ifdef PURE
	if (ltemp <= 0) {
		std::cout << "Capability length not greater than 0" << std::endl;
		raise_trap(EXC_INSTR_ACCESS_FAULT, instr.data());
	}
#else
	assert (ltemp > 0);
#endif

	if (ltemp > (int64_t)(MAX_CAP_ADDR)) return (static_cast<uint32_t>(MAX_CAP_ADDR));
	else return (static_cast<uint32_t>(ltemp));
}


//Replace certain bits of a register with a fixed value
uint32_t ISS::replace_bits(uint32_t source, uint8_t from, uint8_t to, uint32_t replacement) {
	uint32_t replaced = source & REPLACEMENT_MASK(from, to);
	uint32_t replacement_match = BIT_RANGE( ((replacement) << from), to, from );
	replaced |= replacement_match;

	return (replaced);
}


//Does not exactly set the address. Mainly does the fast rep check
//If setoffset is 1, rs2 is added to cs1 base, else to cs1 address
bool ISS::set_offset(Capability cs1, int64_t rs2, bool setoffset) {
	uint8_t E, Amid, B_4_0, R, Imid, diff;
	uint32_t s; 
	uint64_t mod_rs2;
	int8_t Bupper;
	bool tag, inrange, inlimit = 0;

	if ((cs1.caps) & (1 << CAP_IE)) {
		E = ((((cs1.caps) >> CAP_L7) & 1) << 4) | ((BIT_SLICE((cs1.caps), CAP_TE_TOP, CAP_TE_BOT)) << 2) | \
			BIT_RANGE((cs1.caps), CAP_BE_TOP, CAP_BE_BOT);
		if (setoffset) B_4_0 = BIT_RANGE(cs1.caps, (R_BOT - 1), CAP_B_BOT) | 0;
	}
	else {
		E = 0;
		if (setoffset) B_4_0 = BIT_RANGE(cs1.caps, (R_BOT - 1), CAP_BE_BOT) | 0;
	}

	tag = 1;
	if (E < (E_MAX - 2)) {
		s = 1 << (E + R_TOP + 1);
		if (rs2 < 0) {
			//InRange test
			mod_rs2 = static_cast<uint64_t>((-(rs2)));
			inrange = (mod_rs2 < s);

			if (inrange) {
				//InLimit test
				if (setoffset) diff = ((~((1 >> 5) | B_4_0)) + 1);
				else {
				Bupper = BIT_SLICE(cs1.caps, R_TOP, R_BOT);
				//E + CAP_B_TOP cannot exceed 31
				Amid = BIT_SLICE((cs1.addr), (E+CAP_B_TOP), E);
				R = (static_cast<uint8_t>(Bupper - 1) << R_BOT) | 0;
				diff = static_cast<uint8_t>(R - Amid);
				}

				Imid = BIT_SLICE(static_cast<uint64_t>(rs2), (E+R_TOP), E);
				inlimit = ((Imid >= diff) && (R != Amid));
			}
		}

		else {
			//InRange test
			mod_rs2 = static_cast<uint64_t>(rs2);
			inrange = (mod_rs2 < s);

			if (inrange) {
				//InLimit test
				if (setoffset) diff = (~((1 >> 5) | B_4_0));
				else {
				Bupper = BIT_SLICE(cs1.caps, R_TOP, R_BOT);
				//E + CAP_B_TOP cannot exceed 31
				Amid = BIT_SLICE((cs1.addr), (E+CAP_B_TOP), E);
				R = (static_cast<uint8_t>(Bupper - 1) << R_BOT) | 0;
				diff = static_cast<uint8_t>(R - Amid - 1);
				}

				Imid = BIT_SLICE(static_cast<uint64_t>(rs2), (E+7), E);
				if (setoffset) inlimit = (Imid <= diff);
				else inlimit = (Imid < diff);
			}
		}

		tag = (inrange && inlimit); 
	}

	//neu.addr = static_cast<uint32_t>(static_cast<int64_t>(cs1.addr) + rs2);

	return (tag);
}


Capability ISS::seal_cap(Capability cs1, Capability cs2) {
	Capability neu;
	uint32_t cs2_base = get_base(cs2.addr, cs2.caps);
	int64_t cs2_top = get_top(cs2.addr, cs2.caps);
	uint8_t cs1_otype = BIT_SLICE(cs1.caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT);
	uint8_t cs2_otype = BIT_SLICE(cs2.caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT);
	uint32_t cs2_perms = BIT_SLICE(cs2.caps, CAP_PERM_TOP, CAP_PERM_BOT);

	bool permitted = ( (cs2.tag) && (cs2_otype == Unsealed) && (BIT_SINGLE(cs2_perms, PERMIT_SEAL) > 0) && \
					 (cs2.addr >= cs2_base) && (cs2.addr < cs2_top) && (cs2.addr < MAX_CAP_OTYPE) );

	neu.tag = 1;
	if ((cs1_otype != Unsealed) || !(permitted)) neu.tag = 0;

	neu.addr = cs1.addr;
	neu.caps = cs1.caps & REPLACEMENT_MASK(CAP_OTYPE_BOT, CAP_OTYPE_TOP);
	uint32_t replacement_match = BIT_RANGE( ((cs2.addr) << CAP_OTYPE_BOT), CAP_OTYPE_TOP, CAP_OTYPE_BOT );
	neu.caps |= replacement_match;

	return neu;
}


Capability ISS::unseal_cap(Capability cs1, Capability cs2) {
	Capability neu;
	uint32_t neu_perms;
	uint32_t cs2_base = get_base(cs2.addr, cs2.caps);
	int64_t cs2_top = get_top(cs2.addr, cs2.caps);
	uint8_t cs1_otype = BIT_SLICE(cs1.caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT);
	uint8_t cs2_otype = BIT_SLICE(cs2.caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT);
	uint32_t cs1_perms = BIT_SLICE(cs1.caps, CAP_PERM_TOP, CAP_PERM_BOT);
	uint32_t cs2_perms = BIT_SLICE(cs2.caps, CAP_PERM_TOP, CAP_PERM_BOT);

	bool permitted = ( (cs2.tag) && (cs1_otype != Unsealed) && (cs2_otype == Unsealed) && (cs1_otype <= Reserved) && \
					 (cs2.addr == cs1_otype) && (BIT_SINGLE(cs2_perms, PERMIT_UNSEAL) > 0) && \
					 (cs2.addr >= cs2_base) && (cs2.addr < cs2_top) );

	if (BIT_SINGLE(cs1_perms, GLOBAL) & BIT_SINGLE(cs2_perms, GLOBAL)) 
		neu_perms = (SET_BIT(cs1_perms, GLOBAL)) << CAP_PERM_BOT;
	else neu_perms = (UNSET_BIT(cs1_perms, GLOBAL)) << CAP_PERM_BOT;

	neu.tag = 1;
	if (!(permitted)) neu.tag = 0;

	neu.addr = cs1.addr;
	neu.caps = cs1.caps & REPLACEMENT_MASK(CAP_OTYPE_BOT, CAP_OTYPE_TOP) & \
				REPLACEMENT_MASK(CAP_PERM_BOT, CAP_PERM_TOP);
	uint32_t replacement_match = BIT_RANGE( (Unsealed << CAP_OTYPE_BOT), CAP_OTYPE_TOP, CAP_OTYPE_BOT );
	neu.caps |= (replacement_match | neu_perms);

	return neu;
}


//To check if address is within bounds of the capability
bool ISS::cap_inbounds(Capability cs) {
	uint8_t T, B, T_5_2, B_5_2, T_5_0, B_5_0, T_7_6, B_7_6, L_cout, L_msb, E, Amid, R;
	int8_t Bupper;
	bool result = false;

	if (cs.caps & (1 << CAP_IE)) {
		E = (((cs.caps >> CAP_L7) & 1) << 4) | ((BIT_SLICE(cs.caps, CAP_TE_TOP, CAP_TE_BOT)) << 2) | \
			BIT_RANGE(cs.caps, CAP_BE_TOP, CAP_BE_BOT);
		T_5_2 = BIT_SLICE(cs.caps, CAP_T_TOP, CAP_T_BOT);
		B_5_2 = BIT_SLICE(cs.caps, (CAP_B_TOP-2), CAP_B_BOT);
		L_msb = 1;

		if (T_5_2 < B_5_2) L_cout = 1;
		else L_cout = 0;

		B_7_6 = BIT_SLICE(cs.caps, CAP_B_TOP, (CAP_B_TOP-1));
		T_7_6 = B_7_6 + L_cout + L_msb;
		T = (T_7_6 << 6) | (T_5_2 << 2) | 0;
		B = (B_7_6 << (CAP_B_TOP - 1)) | (B_5_2 << CAP_B_BOT) | 0;
	}
	else {
		E = 0;
		T_5_0 = BIT_SLICE(cs.caps, CAP_T_TOP, CAP_TE_BOT);
		B_5_0 = BIT_RANGE(cs.caps, (CAP_B_TOP-2), CAP_BE_BOT);
		L_msb = ((cs.caps >> CAP_L7) & 0b1);

		if (T_5_0 < B_5_0) L_cout = 1;
		else L_cout = 0;

		B_7_6 = BIT_SLICE(cs.caps, CAP_B_TOP, (CAP_B_TOP-1));
		T_7_6 = B_7_6 + L_cout + L_msb;
		T = (T_7_6 << 6) | T_5_0;
		B = (B_7_6 << (CAP_B_TOP - 1)) | (B_5_0 << CAP_BE_BOT) | 0;
	}

	assert (E < (E_MAX+1));
	assert (L_msb==1 || L_msb==0);
	assert (L_cout==1 || L_cout==0);

	Bupper = BIT_SLICE(cs.caps, R_TOP, R_BOT);
	//As E+CAP_B_TOP might exceed 31
	uint64_t Atemp = static_cast<uint64_t>(cs.addr); 
	Amid = BIT_SLICE(Atemp, (E+CAP_B_TOP), E);
	R = (static_cast<uint8_t>(Bupper - 1) << R_BOT) | 0;

	//Check if address and top are in the same segment
	//Then compare Amid with T
	if ((Amid < R) && (T < R)) 
	//Either B in lower segment or Amid >= B
		result = (Amid < T) && ((B >= R) || (Amid >= B));
	else if ((Amid >= R) && (T >= R)) 
	//When everybody in lower segment
		result = (Amid < T) && (Amid >= B);
	//Else top and bottom must be in separate segment and Amid >= B
	else result = (T < R) && (B >= R) && (Amid >= B);


	return (result);
}


SetBound_Ret ISS::set_bounds(Capability cs, uint64_t len, SetBound_flag choice) {
	SetBound_Ret final;
	bool top_modify = false;
	uint8_t Et, E;
	uint32_t new_b = cs.addr;
	uint32_t new_t = (cs.addr + len);

	//First find suitable E
	if ((len >> CAP_B_TOP) == 0) Et = 0; //__builtin_clz is not defined for 0
	//We have to deal with length 2^32 from CBUILDCAP, hence len is taken as a 64 bit value
	else Et = 2*(ADDR_BITS) - (__builtin_clzll(len >> CAP_B_TOP)); 
	if ( (Et < E_MAX) && ( (2*(ADDR_BITS) - (__builtin_clzll(len)) - REG_FIELD_LEN) >=0 ) && \
	(len >> (2*(ADDR_BITS) - (__builtin_clzll(len)) - REG_FIELD_LEN)) == ((0b1 << REG_FIELD_LEN) - 1) )
		E = Et + 1;
	else E = Et;

	//Find suitable mask to get a representable base
	if (E > 0) {
		final.baseMask = (static_cast<uint32_t>(~0) << (E + CAP_B_BOT));
		new_b = cs.addr & (final.baseMask);
		if (BIT_RANGE(new_t, (E+CAP_BE_TOP), CAP_BE_BOT) > 0) {
			top_modify = true;
			new_t = (new_t + (1 << (E+CAP_B_BOT))) & (final.baseMask);
		}
		//else new_t = new_t & (final.baseMask); //not needed as lower part already 0

		if ((new_b == cs.addr) && (!top_modify)) {
			final.exact = true;
			final.newlen = len;
		}
		else {
			final.exact = false;
			final.newlen = (new_t - new_b);
		}
		//Reset new base, top and exponent
		final.cd.caps = cs.caps & REPLACEMENT_MASK(CAP_BE_BOT, (CAP_T_TOP + 1));
		final.cd.caps |= ( (0b1 << CAP_IE) | (BIT_SINGLE_P1(E, (REG_FIELD_LEN-1)) << CAP_L7) | \
						 (BIT_SLICE(E, ((CAP_B_BOT<<1) - 1), CAP_B_BOT) << CAP_TE_BOT) | \
						 (BIT_SLICE(E, CAP_BE_TOP, CAP_BE_BOT) << CAP_BE_BOT) | \
						 (BIT_SLICE(new_b, (E+CAP_B_TOP), (E+CAP_B_BOT)) << CAP_B_BOT) | \
						 (BIT_SLICE(new_t, (E+CAP_B_TOP-2), (E+CAP_B_BOT)) << CAP_T_BOT) );
	}
	else {
		final.baseMask = static_cast<uint32_t>(~0);
		final.exact = true;
		final.newlen = len;

		//Reset new base, top
		final.cd.caps = cs.caps & REPLACEMENT_MASK(CAP_BE_BOT, (CAP_T_TOP + 2)); //IE is 0 as well
		final.cd.caps |= ( (BIT_SINGLE_P1(len, (CAP_B_TOP - 1)) << CAP_L7) | \
						 (BIT_SLICE(new_b, CAP_B_TOP, CAP_BE_BOT) << CAP_BE_BOT) | \
						 (BIT_SLICE(new_t, (CAP_B_TOP-2), CAP_BE_BOT) << CAP_TE_BOT) );
	}

	if ((choice.mask) || (choice.len)) return (final); //Return the mask if only the mask/len is wanted

	final.cd.addr = cs.addr;
	return (final);
}


//Builds a capability from cs2 with cs1 acting as authority
Capability ISS::build_cap(Capability cs1, Capability cs2) {
	Capability requestedCap, cs;
	SetBound_Ret final;
	SetBound_flag choice;
	bool requestedSentry, subset, derivable;
	uint32_t requestedBase;
	int64_t requestedTop;
	int64_t requestedLen;
	uint32_t requestedPerms, authorityPerms;
	uint32_t authorityBase;
	int64_t authorityTop;

	requestedCap.tag = true;
	requestedCap.caps = cs2.caps;
	requestedCap.addr = cs2.addr;

	requestedSentry = (BIT_SLICE((cs2.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT) == Sealed);
	//If sentry is not requested, unseal requested capability
	if (not(requestedSentry)) {
		requestedCap.caps = (requestedCap.caps) & REPLACEMENT_MASK(CAP_OTYPE_BOT, CAP_OTYPE_TOP);
		requestedCap.caps |= (Unsealed << CAP_OTYPE_BOT);
	}

	requestedBase = get_base(requestedCap.addr, requestedCap.caps);
	requestedTop = get_top(requestedCap.addr, requestedCap.caps);
	authorityBase = get_base(cs1.addr, cs1.caps);
	authorityTop = get_top(cs1.addr, cs1.caps);
	//requested top is already int64_t
	//requested length cannot be more than (2^32 - 1)
	requestedLen = requestedTop - static_cast<int64_t>(requestedBase);
	requestedPerms = BIT_SLICE((requestedCap.caps), CAP_PERM_TOP, CAP_PERM_BOT);
	authorityPerms = BIT_SLICE((cs1.caps), CAP_PERM_TOP, CAP_PERM_BOT);

	subset = (requestedLen >= 0) && (requestedBase >= authorityBase) && \
			 (requestedTop <= authorityTop) && \
			 ((requestedPerms & authorityPerms) == requestedPerms);

	cs.addr = requestedBase;
	cs.caps = cs1.caps;
	choice.len = false;
	choice.mask = false;
	final = set_bounds(cs, static_cast<uint64_t>(requestedLen), choice);
	//Set the tag of the authority to derived Capability
	final.cd.tag = cs1.tag;
	//set new address 
	final.cd.addr = requestedCap.addr;
	//set new perms
	final.cd.caps = (final.cd.caps) & REPLACEMENT_MASK(CAP_PERM_BOT, CAP_PERM_TOP);
	final.cd.caps |= (requestedPerms << CAP_PERM_BOT);
	//Set new flags
	if (BIT_SINGLE((requestedCap.caps), CAP_FLAG)) final.cd.caps = SET_BIT((final.cd.caps), CAP_FLAG);
	else final.cd.caps = UNSET_BIT((final.cd.caps), CAP_FLAG);
	//Set Otype to Sentry if requested
	if (requestedSentry) {
		final.cd.caps = (final.cd.caps) & REPLACEMENT_MASK(CAP_OTYPE_BOT, CAP_OTYPE_TOP);
		final.cd.caps |= (Sealed << CAP_OTYPE_BOT);
	}

	derivable = (final.cd.caps == requestedCap.caps) && (final.cd.addr == requestedCap.addr) && \
				(final.cd.tag == requestedCap.tag);

	/*spandan----------------------------------------------------------------------*/
	//Later replace with trap handling
	assert( (not(derivable) | (final.exact)) && "CBuildCap : Capability bounds not exact" );
	/*spandan----------------------------------------------------------------------*/

	if (subset && derivable) return (final.cd);
	else {
		requestedCap.tag = 0;
		return (requestedCap);
	}
}

//Derive a valid pcc from epcc or return an invalid capability if not possible
Capability ISS::legalize_epcc(Capability cs) {
	Capability cd;
	bool fast_rep_check;

#ifdef TESTRIG
	//In TestRIG, always clear the last 2 bits
	cd.addr = cs.addr & (~3u);
#else
	if (csrs.misa.reg & C_ISA_EXT) cd.addr = cs.addr & (~1u);
	else cd.addr = cs.addr & (~3u);
#endif

	cd.caps = cs.caps;
	cd.tag = false;
	if (cs.tag) {
		fast_rep_check = set_offset(cd, 0, false);
		if (fast_rep_check) cd.tag = true;
	}

	return (cd);
}

//Check if a capability is derivable from tcc and if so, replace tcc with it
Capability ISS::legalize_tcc(Capability tcc, Capability cs) {
	Capability cd;

#ifdef TESTRIG
	//In TestRIG, always clear the last 2 bits
	cs.addr = cs.addr & (~3u);
#else
	if (csrs.misa.reg & C_ISA_EXT) cs.addr = cs.addr & (~1u);
	else cs.addr = cs.addr & (~3u);
#endif

	if (BIT_SLICE(tcc.caps, CAP_OTYPE_TOP, CAP_OTYPE_BOT) != Unsealed)
		tcc.tag = false;
	cd = build_cap(tcc, cs);

	return (cd);
}

//Get SCR values
Capability ISS::get_scr(uint8_t scr) {
	Capability cd;
	switch (scr) {
		case 0: {
			cd.addr = last_pc; cd.caps = cpc; cd.tag = pc_tag;
			bool fast_rep_check = set_offset(cd, 0, false);
			assert ( fast_rep_check && "PC should always be representable" );
			} break;
		case 1: 
			cd.addr = pre_ddc; cd.caps = cpre_ddc; cd.tag = pre_ddc_tag;
			break;

		case 4:
			cd.addr = csrs.utvec.reg; cd.caps = csrs.utvec.cheri; cd.tag = csrs.utvec.tag;
			break;
		case 5:
			cd.addr = csrs.utdc.reg; cd.caps = csrs.utdc.cheri; cd.tag = csrs.utdc.tag;
			break;
		case 6:
			cd.addr = csrs.uscratch.reg; cd.caps = csrs.uscratch.cheri; cd.tag = csrs.uscratch.tag;
			break;
		case 7:
			cd = legalize_epcc( Capability{csrs.uepc.reg, csrs.uepc.cheri, csrs.uepc.tag} );
			break;

		case 12:
			cd.addr = csrs.stvec.reg; cd.caps = csrs.stvec.cheri; cd.tag = csrs.stvec.tag;
			break;
		case 13:
			cd.addr = csrs.stdc.reg; cd.caps = csrs.stdc.cheri; cd.tag = csrs.stdc.tag;
			break;
		case 14:
			cd.addr = csrs.sscratch.reg; cd.caps = csrs.sscratch.cheri; cd.tag = csrs.sscratch.tag;
			break;
		case 15:
			cd = legalize_epcc( Capability{csrs.sepc.reg, csrs.sepc.cheri, csrs.sepc.tag} );
			break;

		case 28:
			cd.addr = csrs.mtvec.reg; cd.caps = csrs.mtvec.cheri; cd.tag = csrs.mtvec.tag;
			break;
		case 29:
			cd.addr = csrs.mtdc.reg; cd.caps = csrs.mtdc.cheri; cd.tag = csrs.mtdc.tag;
			break;
		case 30:
			cd.addr = csrs.mscratch.reg; cd.caps = csrs.mscratch.cheri; cd.tag = csrs.mscratch.tag;
			break;
		case 31:
			cd = legalize_epcc( Capability{csrs.mepc.reg, csrs.mepc.cheri, csrs.mepc.tag} );
			break;

		default: assert(false && "Unreachable SCR");
	}

	return (cd);
}


//Set SCR values
bool ISS::set_scr(uint8_t scr, Capability cs) {
	Capability cd;
	
	switch (scr) {
		case 1: 
			pre_ddc = cs.addr; cpre_ddc = cs.caps; pre_ddc_tag = cs.tag;
			break;

		case 4:
			cd = legalize_tcc( Capability{csrs.utvec.reg, csrs.utvec.cheri, csrs.utvec.tag} , cs);
			if (cd.tag)
				{csrs.utvec.reg = cd.addr; csrs.utvec.cheri = cd.caps; csrs.utvec.tag = cd.tag;}
			else return false;
			break;
		case 5:
			csrs.utdc.reg = cs.addr; csrs.utdc.cheri = cs.caps; csrs.utdc.tag = cs.tag;
			break;
		case 6:
			csrs.uscratch.reg = cs.addr; csrs.uscratch.cheri = cs.caps; csrs.uscratch.tag = cs.tag;
			break;
		case 7:
			csrs.uepc.reg = cs.addr; csrs.uepc.cheri = cs.caps; csrs.uepc.tag = cs.tag;
			break;

		case 12:
			cd = legalize_tcc( Capability{csrs.stvec.reg, csrs.stvec.cheri, csrs.stvec.tag} , cs);
			if (cd.tag)
				{csrs.stvec.reg = cd.addr; csrs.stvec.cheri = cd.caps; csrs.stvec.tag = cd.tag;}
			else return false;
			break;
		case 13:
			csrs.stdc.reg = cs.addr; csrs.stdc.cheri = cs.caps; csrs.stdc.tag = cs.tag;
			break;
		case 14:
			csrs.sscratch.reg = cs.addr; csrs.sscratch.cheri = cs.caps; csrs.sscratch.tag = cs.tag;
			break;
		case 15:
			csrs.sepc.reg = cs.addr; csrs.sepc.cheri = cs.caps; csrs.sepc.tag = cs.tag;
			break;

		case 28:
			cd = legalize_tcc( Capability{csrs.mtvec.reg, csrs.mtvec.cheri, csrs.mtvec.tag} , cs);
			if (cd.tag)
				{csrs.mtvec.reg = cd.addr; csrs.mtvec.cheri = cd.caps; csrs.mtvec.tag = cd.tag;}
			else return false;
			break;
		case 29:
			csrs.mtdc.reg = cs.addr; csrs.mtdc.cheri = cs.caps; csrs.mtdc.tag = cs.tag;
			break;
		case 30:
			csrs.mscratch.reg = cs.addr; csrs.mscratch.cheri = cs.caps; csrs.mscratch.tag = cs.tag;
			break;
		case 31:
			csrs.mepc.reg = cs.addr; csrs.mepc.cheri = cs.caps; csrs.mepc.tag = cs.tag;
			break;

		default: 
#ifdef PURE
			std::cout << "Unreachable SCR" << std::endl;
			raise_trap(EXC_INSTR_ACCESS_FAULT, scr);
#else
			assert(false && "Unreachable SCR");
#endif
	}

	return true;
}


//Raise trap for CJALR exceptions
void ISS::check_jalr_exceptions(uint32_t regnum, Capability cs1, int32_t imm) {
	uint32_t xtval_reg = replace_bits( (csrs.mtval.reg), CAP_ID_BOT, CAP_ID_TOP, regnum);
	uint32_t cs1_perms = BIT_SLICE( (cs1.caps), CAP_PERM_TOP, CAP_PERM_BOT);
	uint32_t cs1_base = get_base((cs1.addr), (cs1.caps));
	int64_t cs1_top = get_top((cs1.addr), (cs1.caps));
	uint8_t cs1_otype = BIT_SLICE( (cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT );
	uint32_t basecheck = cs1.addr + static_cast<uint32_t>(imm); // intended wraparound as per specification

	if (not(cs1.tag)) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_TAG_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if ( (cs1_otype != Unsealed) && ((cs1_otype != Sealed) || (imm != 0x0)) ) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_SEAL_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (BIT_SINGLE(cs1_perms, PERMIT_EXECUTE) == 0) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_EXECUTE_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	//Assuming no pcc relocation
	else if ( basecheck < cs1_base ){
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if ( (static_cast<int64_t>(basecheck) + MIN_INSTR_BYTES) > cs1_top ) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else return;
}


//Raise trap for CJAL exceptions
void ISS::check_jal_exceptions(uint32_t regnum, Capability cd, int32_t imm) {
	uint32_t xtval_reg = replace_bits( (csrs.mtval.reg), CAP_ID_BOT, CAP_ID_TOP, regnum);
	uint32_t cd_base = get_base((cd.addr), (cd.caps));
	int64_t cd_top = get_top((cd.addr), (cd.caps));
	uint32_t basecheck = cd.addr + static_cast<uint32_t>(imm); // intended wraparound as per specification

	if ( basecheck < cd_base ){
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if ( (static_cast<int64_t>(basecheck) + MIN_INSTR_BYTES) > cd_top ) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else return;
}


//Raise trap for CLOADTAGS exceptions
void ISS::check_ltags_exceptions(uint32_t regnum, Capability cs1) {
	uint32_t xtval_reg = replace_bits( (csrs.mtval.reg), CAP_ID_BOT, CAP_ID_TOP, regnum);
	uint32_t cs1_perms = BIT_SLICE( (cs1.caps), CAP_PERM_TOP, CAP_PERM_BOT);
	uint32_t cs1_base = get_base((cs1.addr), (cs1.caps));
	int64_t cs1_top = get_top((cs1.addr), (cs1.caps));
	uint8_t cs1_otype = BIT_SLICE( (cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT );

	//Debug purposes
	std::cerr
    << std::hex
    << "addr=" << (uint32_t)cs1.addr
    << " base=" << cs1_base
    << " top=" << cs1_top
    << " tag=" << (int)cs1.tag
    << " perms=" << cs1_perms
    << std::endl;

	if (not(cs1.tag)) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_TAG_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (cs1_otype != Unsealed) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_SEAL_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (BIT_SINGLE(cs1_perms, PERMIT_LOAD) == 0) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_LOAD_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (BIT_SINGLE(cs1_perms, PERMIT_LOAD_CAP) == 0) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_LOAD_CAP_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	//Assuming no pcc relocation
	else if ( (cs1.addr) < cs1_base ){
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if ( ((int64_t)(cs1.addr) + (CAP_LEN/8)) > cs1_top ) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else return;
}


//Raise trap for instruction fetch
void ISS::check_fetch_exceptions(uint32_t regnum, Capability cs) {
	uint32_t xtval_reg = replace_bits( (csrs.mtval.reg), CAP_ID_BOT, CAP_ID_TOP, regnum);
	uint32_t cs_perms = BIT_SLICE( (cs.caps), CAP_PERM_TOP, CAP_PERM_BOT);
	uint32_t cs_base = get_base((cs.addr), (cs.caps));
	int64_t cs_top = get_top((cs.addr), (cs.caps));
	uint8_t cs_otype = BIT_SLICE( (cs.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT );
	uint32_t basecheck = cs.addr;

	if (not(cs.tag)) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_TAG_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (cs_otype != Unsealed) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_SEAL_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (BIT_SINGLE(cs_perms, PERMIT_LOAD) == 0) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_LOAD_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (BIT_SINGLE(cs_perms, PERMIT_EXECUTE) == 0) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_EXECUTE_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if ( basecheck < cs_base ){
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if ( (static_cast<int64_t>(basecheck) + MIN_INSTR_BYTES) > cs_top ) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else return;
}


//Raise trap for [C]LC exceptions
void ISS::check_clc_exceptions(uint32_t regnum, Capability cs1, int64_t imm) {
	uint32_t xtval_reg = replace_bits( (csrs.mtval.reg), CAP_ID_BOT, CAP_ID_TOP, regnum);
	uint32_t cs1_perms = BIT_SLICE( (cs1.caps), CAP_PERM_TOP, CAP_PERM_BOT);
	uint32_t cs1_base = get_base((cs1.addr), (cs1.caps));
	int64_t cs1_top = get_top((cs1.addr), (cs1.caps));
	uint8_t cs1_otype = BIT_SLICE( (cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT );
	uint32_t basecheck = cs1.addr + static_cast<uint32_t>(imm); // intended wraparound as per specification

	if (not(cs1.tag)) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_TAG_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (cs1_otype != Unsealed) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_SEAL_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (BIT_SINGLE(cs1_perms, PERMIT_LOAD) == 0) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_LOAD_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if ( basecheck < cs1_base ){
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if ( (static_cast<int64_t>(basecheck) + (CAP_LEN/8)) > cs1_top ) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else return;
}


//Raise trap for data load exceptions
void ISS::check_dataload_exceptions(uint32_t regnum, Capability cs1, uint8_t size, int64_t imm) {
	uint32_t xtval_reg = replace_bits( (csrs.mtval.reg), CAP_ID_BOT, CAP_ID_TOP, regnum);
	uint32_t cs1_perms = BIT_SLICE( (cs1.caps), CAP_PERM_TOP, CAP_PERM_BOT);
	uint32_t cs1_base = get_base((cs1.addr), (cs1.caps));
	int64_t cs1_top = get_top((cs1.addr), (cs1.caps));
	uint8_t cs1_otype = BIT_SLICE( (cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT );
	uint32_t basecheck = cs1.addr + static_cast<uint32_t>(imm); // intended wraparound as per specification

	if (not(cs1.tag)) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_TAG_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (cs1_otype != Unsealed) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_SEAL_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (BIT_SINGLE(cs1_perms, PERMIT_LOAD) == 0) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_LOAD_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if ( basecheck < cs1_base ){
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if ( (static_cast<int64_t>(basecheck) + size) > cs1_top ) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else return;
}


//Raise trap for [C]SC exceptions
void ISS::check_csc_exceptions(uint32_t regnum1, uint32_t regnum2, Capability cs1, Capability cs2, int64_t imm) {
	uint32_t xtval_reg1 = replace_bits( (csrs.mtval.reg), CAP_ID_BOT, CAP_ID_TOP, regnum1);
	uint32_t xtval_reg2 = replace_bits( (csrs.mtval.reg), CAP_ID_BOT, CAP_ID_TOP, regnum2);
	uint32_t cs1_perms = BIT_SLICE( (cs1.caps), CAP_PERM_TOP, CAP_PERM_BOT );
	uint32_t cs2_perms = BIT_SLICE( (cs2.caps), CAP_PERM_TOP, CAP_PERM_BOT );
	uint8_t cs1_otype = BIT_SLICE( (cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT );

	uint32_t cs1_base = get_base((cs1.addr), (cs1.caps));
	int64_t cs1_top = get_top((cs1.addr), (cs1.caps));
	uint32_t basecheck = cs1.addr + static_cast<uint32_t>(imm); // intended wraparound as per specification

	if (not(cs1.tag)) {
		xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_TAG_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg1);
	}
	else if (cs1_otype != Unsealed) {
		xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_SEAL_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg1);
	}
	else if (BIT_SINGLE(cs1_perms, PERMIT_STORE) == 0) {
		xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_STORE_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg1);
	}
	else if (BIT_SINGLE(cs1_perms, PERMIT_STORE_CAP) == 0) {
		if (cs2.tag) {
			xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_STORE_CAP_VIOLATE);
			raise_trap(EXC_CHERI, xtval_reg1);
		}
	}
	else if (BIT_SINGLE(cs1_perms, PERMIT_STORE_LOCAL_CAP) == 0) {
		if ((cs2.tag) && (BIT_SINGLE(cs2_perms, GLOBAL) == 0)) {
			xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_STORE_LOCAL_CAP_VIOLATE);
			xtval_reg2 = replace_bits(xtval_reg2, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_GLOBAL_VIOLATE);
			raise_trap(EXC_CHERI, xtval_reg1);
			raise_trap(EXC_CHERI, xtval_reg2);
		}
	}
	else if ( basecheck < cs1_base ){
		xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg1);
	}
	else if ( (static_cast<int64_t>(basecheck) + (CAP_LEN/8)) > cs1_top ) {
		xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg1);
	}
	else return;
}


//Raise trap for data store exceptions
void ISS::check_datastore_exceptions(uint32_t regnum, Capability cs1, uint8_t size, int64_t imm) {
	uint32_t xtval_reg = replace_bits( (csrs.mtval.reg), CAP_ID_BOT, CAP_ID_TOP, regnum);
	uint32_t cs1_perms = BIT_SLICE( (cs1.caps), CAP_PERM_TOP, CAP_PERM_BOT);
	uint32_t cs1_base = get_base((cs1.addr), (cs1.caps));
	int64_t cs1_top = get_top((cs1.addr), (cs1.caps));
	uint8_t cs1_otype = BIT_SLICE( (cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT );
	uint32_t basecheck = cs1.addr + static_cast<uint32_t>(imm); // intended wraparound as per specification

	if (not(cs1.tag)) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_TAG_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (cs1_otype != Unsealed) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_SEAL_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if (BIT_SINGLE(cs1_perms, PERMIT_STORE) == 0) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_STORE_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if ( basecheck < cs1_base ){
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else if ( (static_cast<int64_t>(basecheck) + size) > cs1_top ) {
		xtval_reg = replace_bits(xtval_reg, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg);
	}
	else return;
}


//Raise trap for CINVOKE exceptions
void ISS::check_invoke_exceptions(uint32_t regnum1, uint32_t regnum2, Capability cs1, Capability cs2) {
	uint32_t xtval_reg1 = replace_bits( (csrs.mtval.reg), CAP_ID_BOT, CAP_ID_TOP, regnum1);
	uint32_t xtval_reg2 = replace_bits( (csrs.mtval.reg), CAP_ID_BOT, CAP_ID_TOP, regnum2);
	uint32_t cs1_perms = BIT_SLICE( (cs1.caps), CAP_PERM_TOP, CAP_PERM_BOT );
	uint32_t cs2_perms = BIT_SLICE( (cs2.caps), CAP_PERM_TOP, CAP_PERM_BOT );
	uint8_t cs1_otype = BIT_SLICE( (cs1.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT );
	uint8_t cs2_otype = BIT_SLICE( (cs2.caps), CAP_OTYPE_TOP, CAP_OTYPE_BOT );

	uint32_t cs1_base = get_base((cs1.addr), (cs1.caps));
	int64_t cs1_top = get_top((cs1.addr), (cs1.caps));

	if (not(cs1.tag)) {
		xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_TAG_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg1);
	}
	else if (not(cs2.tag)) {
		xtval_reg2 = replace_bits(xtval_reg2, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_TAG_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg2);
	}
	else if (cs1_otype > Reserved) {
		xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_SEAL_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg1);
	}
	else if (cs2_otype > Reserved) {
		xtval_reg2 = replace_bits(xtval_reg2, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_SEAL_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg2);
	}
	else if (cs1_otype != cs2_otype) {
		xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_TYPE_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg1);
	}
	else if (BIT_SINGLE(cs1_perms, PERMIT_INVOKE) == 0) {
		xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_INVOKE_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg1);
	}
	else if (BIT_SINGLE(cs2_perms, PERMIT_INVOKE) == 0) {
		xtval_reg2 = replace_bits(xtval_reg2, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_INVOKE_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg2);
	}
	else if (BIT_SINGLE(cs1_perms, PERMIT_EXECUTE) == 0) {
		xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_EXECUTE_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg1);
	}
	else if (BIT_SINGLE(cs2_perms, PERMIT_EXECUTE) == 0) {
		xtval_reg2 = replace_bits(xtval_reg2, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_PERM_EXECUTE_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg2);
	}
	//Assuming no pcc relocation
	else if ( (cs1.addr) < cs1_base ){
		xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg1);
	}
	else if ( ((int64_t)(cs1.addr) + MIN_INSTR_BYTES) > cs1_top ) {
		xtval_reg1 = replace_bits(xtval_reg1, CAP_CAUSE_BOT, CAP_CAUSE_TOP, EXC_LENGTH_VIOLATE);
		raise_trap(EXC_CHERI, xtval_reg1);
	}
	else return;
}
/*spandan------------------------------------------------------------------*/

uint64_t ISS::get_progam_counter(void) {
    return pc;
}

void ISS::block_on_wfi(bool block) {
    ignore_wfi = !block;
}

CoreExecStatus ISS::get_status(void) {
    return status;
}

void ISS::set_status(CoreExecStatus s) {
    status = s;
}

void ISS::enable_debug(void) {
    debug_mode = true;
}

void ISS::insert_breakpoint(uint64_t addr) {
    breakpoints.insert(addr);
}

void ISS::remove_breakpoint(uint64_t addr) {
    breakpoints.erase(addr);
}

uint64_t ISS::get_hart_id() {
    return csrs.mhartid.reg;
}

std::vector<uint64_t> ISS::get_registers(void) {
    std::vector<uint64_t> regvals;

    for (auto v : regs.regs)
        regvals.push_back((uint32_t)v); //NOTE: zero extend

    return regvals;
}


void ISS::fp_finish_instr() {
	fp_set_dirty();
	fp_update_exception_flags();
}

void ISS::fp_prepare_instr() {
	assert(softfloat_exceptionFlags == 0);
	fp_require_not_off();
}

void ISS::fp_set_dirty() {
	csrs.mstatus.fields.sd = 1;
	csrs.mstatus.fields.fs = FS_DIRTY;
}

void ISS::fp_update_exception_flags() {
	if (softfloat_exceptionFlags) {
		fp_set_dirty();
		csrs.fcsr.fields.fflags |= softfloat_exceptionFlags;
		softfloat_exceptionFlags = 0;
	}
}

void ISS::fp_setup_rm() {
	auto rm = instr.frm();
	if (rm == FRM_DYN)
		rm = csrs.fcsr.fields.frm;
	if (rm >= FRM_RMM)
		RAISE_ILLEGAL_INSTRUCTION();
	softfloat_roundingMode = rm;
}

void ISS::fp_require_not_off() {
	if (csrs.mstatus.fields.fs == FS_OFF)
		RAISE_ILLEGAL_INSTRUCTION();
}

void ISS::return_from_trap_handler(PrivilegeLevel return_mode) {
	switch (return_mode) {
		case MachineMode:
			prv = csrs.mstatus.fields.mpp;
			csrs.mstatus.fields.mie = csrs.mstatus.fields.mpie;
			csrs.mstatus.fields.mpie = 1;
			pc = csrs.mepc.reg;
			if (csrs.misa.has_user_mode_extension())
				csrs.mstatus.fields.mpp = UserMode;
			else
				csrs.mstatus.fields.mpp = MachineMode;
			break;

		case SupervisorMode:
			prv = csrs.mstatus.fields.spp;
			csrs.mstatus.fields.sie = csrs.mstatus.fields.spie;
			csrs.mstatus.fields.spie = 1;
			pc = csrs.sepc.reg;
			if (csrs.misa.has_user_mode_extension())
				csrs.mstatus.fields.spp = UserMode;
			else
				csrs.mstatus.fields.spp = SupervisorMode;
			break;

		case UserMode:
			prv = UserMode;
			csrs.mstatus.fields.uie = csrs.mstatus.fields.upie;
			csrs.mstatus.fields.upie = 1;
			pc = csrs.uepc.reg;
			break;

		default:
			throw std::runtime_error("unknown privilege level " + std::to_string(return_mode));
	}

#ifdef TESTRIG
	// Write new pc to the return packet and set trap
	rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif

	if (trace)
		printf("[vp::iss] return from trap handler, time %s, pc %8x, prv %1x\n",
		       quantum_keeper.get_current_time().to_string().c_str(), pc, prv);
}

void ISS::trigger_external_interrupt(PrivilegeLevel level) {
	if (trace)
		std::cout << "[vp::iss] trigger external interrupt, " << sc_core::sc_time_stamp() << std::endl;

	switch (level) {
		case UserMode:
			csrs.mip.fields.ueip = true;
			break;
		case SupervisorMode:
			csrs.mip.fields.seip = true;
			break;
		case MachineMode:
			csrs.mip.fields.meip = true;
			break;
	}

	wfi_event.notify(sc_core::SC_ZERO_TIME);
}

void ISS::clear_external_interrupt(PrivilegeLevel level) {
	if (trace)
		std::cout << "[vp::iss] clear external interrupt, " << sc_core::sc_time_stamp() << std::endl;

	switch (level) {
		case UserMode:
			csrs.mip.fields.ueip = false;
			break;
		case SupervisorMode:
			csrs.mip.fields.seip = false;
			break;
		case MachineMode:
			csrs.mip.fields.meip = false;
			break;
	}
}

void ISS::trigger_timer_interrupt(bool status) {
	if (trace)
		std::cout << "[vp::iss] trigger timer interrupt=" << status << ", " << sc_core::sc_time_stamp() << std::endl;
	csrs.mip.fields.mtip = status;
	wfi_event.notify(sc_core::SC_ZERO_TIME);
}

void ISS::trigger_software_interrupt(bool status) {
	if (trace)
		std::cout << "[vp::iss] trigger software interrupt=" << status << ", " << sc_core::sc_time_stamp() << std::endl;
	csrs.mip.fields.msip = status;
	wfi_event.notify(sc_core::SC_ZERO_TIME);
}

PrivilegeLevel ISS::prepare_trap(SimulationTrap &e) {
	// undo any potential pc update (for traps the pc should point to the originating instruction and not it's
	// successor)
	pc = last_pc;
	unsigned exc_bit = (1 << e.reason);

	// 1) machine mode execution takes any traps, independent of delegation setting
	// 2) non-delegated traps are processed in machine mode, independent of current execution mode
	if (prv == MachineMode || !(exc_bit & csrs.medeleg.reg)) {
		csrs.mcause.fields.interrupt = 0;
		csrs.mcause.fields.exception_code = e.reason;
		csrs.mtval.reg = boost::lexical_cast<uint32_t>(e.mtval);
		return MachineMode;
	}

	// see above machine mode comment
	if (prv == SupervisorMode || !(exc_bit & csrs.sedeleg.reg)) {
		csrs.scause.fields.interrupt = 0;
		csrs.scause.fields.exception_code = e.reason;
		csrs.stval.reg = boost::lexical_cast<uint32_t>(e.mtval);
		return SupervisorMode;
	}

	assert(prv == UserMode && (exc_bit & csrs.medeleg.reg) && (exc_bit & csrs.sedeleg.reg));
	csrs.ucause.fields.interrupt = 0;
	csrs.ucause.fields.exception_code = e.reason;
	csrs.utval.reg = boost::lexical_cast<uint32_t>(e.mtval);
	return UserMode;
}

void ISS::prepare_interrupt(const PendingInterrupts &e) {
	if (trace) {
		std::cout << "[vp::iss] prepare interrupt, pending=" << e.pending << ", target-mode=" << e.target_mode
		          << std::endl;
	}

	csr_mip x{e.pending};

	ExceptionCode exc;
	if (x.fields.meip)
		exc = EXC_M_EXTERNAL_INTERRUPT;
	else if (x.fields.msip)
		exc = EXC_M_SOFTWARE_INTERRUPT;
	else if (x.fields.mtip)
		exc = EXC_M_TIMER_INTERRUPT;
	else if (x.fields.seip)
		exc = EXC_S_EXTERNAL_INTERRUPT;
	else if (x.fields.ssip)
		exc = EXC_S_SOFTWARE_INTERRUPT;
	else if (x.fields.stip)
		exc = EXC_S_TIMER_INTERRUPT;
	else if (x.fields.ueip)
		exc = EXC_U_EXTERNAL_INTERRUPT;
	else if (x.fields.usip)
		exc = EXC_U_SOFTWARE_INTERRUPT;
	else if (x.fields.utip)
		exc = EXC_U_TIMER_INTERRUPT;
	else
		throw std::runtime_error("some pending interrupt must be available here");

	switch (e.target_mode) {
		case MachineMode:
			csrs.mcause.fields.exception_code = exc;
			csrs.mcause.fields.interrupt = 1;
			break;

		case SupervisorMode:
			csrs.scause.fields.exception_code = exc;
			csrs.scause.fields.interrupt = 1;
			break;

		case UserMode:
			csrs.ucause.fields.exception_code = exc;
			csrs.ucause.fields.interrupt = 1;
			break;

		default:
			throw std::runtime_error("unknown privilege level " + std::to_string(e.target_mode));
	}
}

PendingInterrupts ISS::compute_pending_interrupts() {
	uint32_t pending = csrs.mie.reg & csrs.mip.reg;

	if (!pending)
		return {NoneMode, 0};

	auto m_pending = pending & ~csrs.mideleg.reg;
	if (m_pending && (prv < MachineMode || (prv == MachineMode && csrs.mstatus.fields.mie))) {
		return {MachineMode, m_pending};
	}

	pending = pending & csrs.mideleg.reg;
	auto s_pending = pending & ~csrs.sideleg.reg;
	if (s_pending && (prv < SupervisorMode || (prv == SupervisorMode && csrs.mstatus.fields.sie))) {
		return {SupervisorMode, s_pending};
	}

	auto u_pending = pending & csrs.sideleg.reg;
	if (u_pending && (prv == UserMode && csrs.mstatus.fields.uie)) {
		return {UserMode, u_pending};
	}

	return {NoneMode, 0};
}

void ISS::switch_to_trap_handler(PrivilegeLevel target_mode) {
	if (trace) {
		printf("[vp::iss] switch to trap handler, time %s, last_pc %8x, pc %8x, irq %u, t-prv %1x\n",
		       quantum_keeper.get_current_time().to_string().c_str(), last_pc, pc, csrs.mcause.fields.interrupt, target_mode);
	}

#ifdef TESTRIG
	rvfi_dii_output.rvfi_dii_trap = 1;
#endif

	// free any potential LR/SC bus lock before processing a trap/interrupt
	release_lr_sc_reservation();

	auto pp = prv;
	prv = target_mode;

	uint8_t mtval_cause, stval_cause, utval_cause; //spandan

	switch (target_mode) {
		case MachineMode: {

		/*spandan------------------------------------------------------*/
			//Check CHERI exceptions
			if (csrs.mcause.fields.exception_code == EXC_CHERI) {
				mtval_cause = BIT_SLICE( (csrs.mtval.reg), CAP_CAUSE_TOP, CAP_CAUSE_BOT );
				cheri_exceptions(mtval_cause, csrs.mtval.reg);
			}
		/*spandan------------------------------------------------------*/

			csrs.mepc.reg = pc;
			csrs.mepc.cheri = cpc; //spandan
			csrs.mepc.tag = pc_tag; //spandan

			csrs.mstatus.fields.mpie = csrs.mstatus.fields.mie;
			csrs.mstatus.fields.mie = 0;
			csrs.mstatus.fields.mpp = pp;

#if !defined(NOTAGROOT)
			pc = csrs.mtvec.get_base_address();
			cpc = csrs.mtvec.cheri;
			pc_tag = csrs.mtvec.tag;

			if(pc == 0) {
				if(error_on_zero_traphandler) {
					throw std::runtime_error("[ISS] Took null trap handler in machine mode");
				} else {
					static bool once = true;
					if (once)
						std::cout << "[ISS] Warn: Taking trap handler in machine mode to 0x0, this is probably an error." << std::endl;
					once = false;
				}
			}
#else
			pc += 4;
#endif

#ifdef TESTRIG
			//For TestRIG, pc is a capability
			pc = csrs.mtvec.get_base_address();
			cpc = csrs.mtvec.cheri;
			pc_tag = csrs.mtvec.tag;
			rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif

			if (csrs.mcause.fields.interrupt && csrs.mtvec.fields.mode == csr_mtvec::Mode::Vectored)
				pc += 4 * csrs.mcause.fields.exception_code;
		}
		break;

		case SupervisorMode: {

		/*spandan------------------------------------------------------*/
			//Check CHERI exceptions
			if (csrs.scause.fields.exception_code == EXC_CHERI) {
				stval_cause = BIT_SLICE( (csrs.stval.reg), CAP_CAUSE_TOP, CAP_CAUSE_BOT );
				cheri_exceptions(stval_cause, csrs.stval.reg);
			}	
		/*spandan------------------------------------------------------*/
			assert(prv == SupervisorMode || prv == UserMode);

			csrs.sepc.reg = pc;
			csrs.sepc.cheri = cpc; //spandan
			csrs.sepc.tag = pc_tag; //spandan

			csrs.mstatus.fields.spie = csrs.mstatus.fields.sie;
			csrs.mstatus.fields.sie = 0;
			csrs.mstatus.fields.spp = pp;

#ifdef NOTAGROOT
			pc += 4;
#endif

#ifdef TESTRIG
			//For TestRIG, pc is a capability
			pc = csrs.stvec.get_base_address();
			cpc = csrs.stvec.cheri;
			pc_tag = csrs.stvec.tag;
			rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif

			if (csrs.scause.fields.interrupt && csrs.stvec.fields.mode == csr_mtvec::Mode::Vectored)
				pc += 4 * csrs.scause.fields.exception_code;
		}
		break;

		case UserMode: {

		/*spandan------------------------------------------------------*/
			//Check CHERI exceptions
			if (csrs.ucause.fields.exception_code == EXC_CHERI) {
				utval_cause = BIT_SLICE( (csrs.utval.reg), CAP_CAUSE_TOP, CAP_CAUSE_BOT );
				cheri_exceptions(utval_cause, csrs.utval.reg);
			}	
		/*spandan------------------------------------------------------*/
		
			assert(prv == UserMode);

			csrs.uepc.reg = pc;
			csrs.uepc.cheri = cpc; //spandan
			csrs.uepc.tag = pc_tag; //spandan

			csrs.mstatus.fields.upie = csrs.mstatus.fields.uie;
			csrs.mstatus.fields.uie = 0;

#ifdef NOTAGROOT
			pc += 4;
#endif

#ifdef TESTRIG
			//For TestRIG, pc is a capability
			pc = csrs.utvec.get_base_address();
			cpc = csrs.utvec.cheri;
			pc_tag = csrs.utvec.tag;
			rvfi_dii_output.rvfi_dii_pc_wdata = static_cast<uint64_t>(pc);
#endif

			if (csrs.ucause.fields.interrupt && csrs.utvec.fields.mode == csr_mtvec::Mode::Vectored)
				pc += 4 * csrs.ucause.fields.exception_code;
		}
		break;

		default:
			throw std::runtime_error("unknown privilege level " + std::to_string(target_mode));
	}
}

void ISS::performance_and_sync_update(Opcode::Mapping executed_op) {
    ++total_num_instr;

	if (!csrs.mcountinhibit.fields.IR)
		++csrs.instret.reg;

	if (lr_sc_counter != 0) {
		--lr_sc_counter;
		assert (lr_sc_counter >= 0);
		if (lr_sc_counter == 0)
            release_lr_sc_reservation();
	}

	auto new_cycles = instr_cycles[executed_op];

	if (!csrs.mcountinhibit.fields.CY)
		cycle_counter += new_cycles;

	quantum_keeper.inc(new_cycles);
	if (quantum_keeper.need_sync()) {
	    if (lr_sc_counter == 0) // match SystemC sync with bus unlocking in a tight LR_W/SC_W loop
		    quantum_keeper.sync();
	}

#ifdef TESTRIG
		rvfi_dii_output.rvfi_dii_order = csrs.instret.reg;
#endif
}

void ISS::run_step() {
	assert(regs.read(0) == 0);

	// speeds up the execution performance (non debug mode) significantly by
	// checking the additional flag first
	if (debug_mode && (breakpoints.find(pc) != breakpoints.end())) {
		status = CoreExecStatus::HitBreakpoint;
		return;
	}

	last_pc = pc;
	try {
		exec_step();

		auto x = compute_pending_interrupts();
		if (x.target_mode != NoneMode) {
			prepare_interrupt(x);
			switch_to_trap_handler(x.target_mode);
		}
	} catch (SimulationTrap &e) {
		if (trace)
			std::cout << "take trap " << e.reason << ", mtval=" << e.mtval << std::endl;
		auto target_mode = prepare_trap(e);
		switch_to_trap_handler(target_mode);
	}

	// NOTE: writes to zero register are supposedly allowed but must be ignored
	// (reset it after every instruction, instead of checking *rd != zero*
	// before every register write)
	regs.regs[regs.zero] = 0;
	/*spandan : x0 should always be CNULL----------*/
	regs.cheri[regs.zero] = NULL_CAP;
	regs.tags = UNSET_BIT( regs.tags, regs.zero );
	/*spandan : x0 should always be CNULL----------*/

	// In case of write to x0, reset the rvfi_dii_output too
	/*
	if (rvfi_dii_output.rvfi_dii_rd_addr == 0) 
		rvfi_dii_output.rvfi_dii_rd_wdata = 0; //spandan
	*/

	// Do not use a check *pc == last_pc* here. The reason is that due to
	// interrupts *pc* can be set to *last_pc* accidentally (when jumping back
	// to *mepc*).
	if (shall_exit)
		status = CoreExecStatus::Terminated;

	performance_and_sync_update(op);
}

void ISS::run() {
	// run a single step until either a breakpoint is hit or the execution
	// terminates
#ifdef TESTRIG
	uint8_t command; //command from TestRIG
	reset_core(); //reset to default state for TestRIG
#endif
	do {
#ifdef TESTRIG
		rvfi_dii_output = {}; //reset the output struct to 0
		rvfi_dii->read_trace(&rvfi_dii_input);
		command = rvfi_dii_input.rvfi_dii_cmd;

		switch (command) {
			case 0: {
				std::cout << "TestRIG reset command received" << std::endl;
				rvfi_dii_output.rvfi_dii_halt = 1;
				rvfi_dii->write_trace(&rvfi_dii_output);
				reset_core();
			}
			continue;

			case 'B':
				command = 0;
				fprintf(stderr, "*BLINK*\n");
				break;

			case 1: {
				std::cout << "TestRIG instruction: " \
				<< std::hex << rvfi_dii_input.rvfi_dii_insn << std::endl;
				// Read the instruction in exec_step()
				// instr = Instruction(rvfi_dii_input.rvfi_dii_insn);
			}
			break;

			case 'Q':
				status = CoreExecStatus::Terminated;
				break;

			default:
				std::cout << "RVFI got unsupported command: " << command << std::endl;
				status = CoreExecStatus::Terminated;
				break;
		}
#endif
		run_step();

#ifdef TESTRIG
		// write back to the port after executing the instruction
		rvfi_dii->write_trace(&rvfi_dii_output);
#endif
	} while (status == CoreExecStatus::Runnable);

	// force sync to make sure that no action is missed
	quantum_keeper.sync();
}

void ISS::show() {
	boost::io::ios_flags_saver ifs(std::cout);
	std::cout << "=[ core : " << csrs.mhartid.reg << " ]===========================" << std::endl;
	std::cout << "simulation time: " << sc_core::sc_time_stamp() << std::endl;
	regs.show();
	//std::cout << "ddc = " << std::hex << pc << std::endl; //spandan
/*spandan------------------------------------------------------*/
	std::cout << "pc = " << std::hex << last_pc; 
	std::cout << "\tnext_pc = " << std::hex << pc;
	std::cout << "\tcpc = " << std::hex << cpc;
	std::cout << "\tpc_tag = " << std::hex << pc_tag << std::endl;
	std::cout << "ddc = " << std::hex << pre_ddc; 
	std::cout << "\tcddc = " << std::hex << cpre_ddc; 
	std::cout << "\tddc_tag = " << std::hex << pre_ddc_tag << std::endl;
/*spandan------------------------------------------------------*/
	std::cout << "num-instr = " << std::dec << csrs.instret.reg << std::endl;
}
