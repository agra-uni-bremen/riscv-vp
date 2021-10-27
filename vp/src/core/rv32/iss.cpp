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

int regcolors[] = {
#if defined(COLOR_THEME_DARK)
    0,  1,  2,  3,  4,  5,  6,  52, 8,  9,  53, 54, 55, 56, 57, 58,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
#elif defined(COLOR_THEME_LIGHT)
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 153, 154, 155, 156, 157, 158,
    116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
#else

#endif
};

RegFile::RegFile() {
	memset(regs, 0, sizeof(regs));
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
	for (unsigned i = 0; i < NUM_REGS; ++i) {
		printf(COLORFRMT " = %8x\n", COLORPRINT(regcolors[i], regnames[i]), regs[i]);
	}
}

ISS::ISS(sc_core::sc_module_name, uint32_t hart_id, bool use_E_base_isa) : 
	systemc_name("Core-" + std::to_string(hart_id)) {
	tsock.register_b_transport(this, &ISS::transport);
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
	assert(((pc & ~pc_alignment_mask()) == 0) && "misaligned instruction");

	try {
		uint32_t mem_word = instr_mem->load_instr(pc);
		instr = Instruction(mem_word);
	} catch (SimulationTrap &e) {
		op = Opcode::UNDEF;
		instr = Instruction(0);
		throw;
	}

	if (instr.is_compressed()) {
		op = instr.decode_and_expand_compressed(RV32);
		pc += 2;
        if (op != Opcode::UNDEF)
            REQUIRE_ISA(C_ISA_EXT);
    } else {
		op = instr.decode_normal(RV32);
		pc += 4;
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
				std::cout << "WARNING: unknown instruction '" << std::to_string(instr.data()) << "' at address '"
				          << std::to_string(last_pc) << "'" << std::endl;
			raise_trap(EXC_ILLEGAL_INSTR, instr.data());
			break;

		case Opcode::ADDI:
			regs[instr.rd()] = regs[instr.rs1()] + instr.I_imm();
			break;

		case Opcode::SLTI:
			regs[instr.rd()] = regs[instr.rs1()] < instr.I_imm();
			break;

		case Opcode::SLTIU:
			regs[instr.rd()] = ((uint32_t)regs[instr.rs1()]) < ((uint32_t)instr.I_imm());
			break;

		case Opcode::XORI:
			regs[instr.rd()] = regs[instr.rs1()] ^ instr.I_imm();
			break;

		case Opcode::ORI:
			regs[instr.rd()] = regs[instr.rs1()] | instr.I_imm();
			break;

		case Opcode::ANDI:
			regs[instr.rd()] = regs[instr.rs1()] & instr.I_imm();
			break;

		case Opcode::ADD:
			regs[instr.rd()] = regs[instr.rs1()] + regs[instr.rs2()];
			break;

		case Opcode::SUB:
			regs[instr.rd()] = regs[instr.rs1()] - regs[instr.rs2()];
			break;

		case Opcode::SLL:
			regs[instr.rd()] = regs[instr.rs1()] << regs.shamt(instr.rs2());
			break;

		case Opcode::SLT:
			regs[instr.rd()] = regs[instr.rs1()] < regs[instr.rs2()];
			break;

		case Opcode::SLTU:
			regs[instr.rd()] = ((uint32_t)regs[instr.rs1()]) < ((uint32_t)regs[instr.rs2()]);
			break;

		case Opcode::SRL:
			regs[instr.rd()] = ((uint32_t)regs[instr.rs1()]) >> regs.shamt(instr.rs2());
			break;

		case Opcode::SRA:
			regs[instr.rd()] = regs[instr.rs1()] >> regs.shamt(instr.rs2());
			break;

		case Opcode::XOR:
			regs[instr.rd()] = regs[instr.rs1()] ^ regs[instr.rs2()];
			break;

		case Opcode::OR:
			regs[instr.rd()] = regs[instr.rs1()] | regs[instr.rs2()];
			break;

		case Opcode::AND:
			regs[instr.rd()] = regs[instr.rs1()] & regs[instr.rs2()];
			break;

		case Opcode::SLLI:
			regs[instr.rd()] = regs[instr.rs1()] << instr.shamt();
			break;

		case Opcode::SRLI:
			regs[instr.rd()] = ((uint32_t)regs[instr.rs1()]) >> instr.shamt();
			break;

		case Opcode::SRAI:
			regs[instr.rd()] = regs[instr.rs1()] >> instr.shamt();
			break;

		case Opcode::LUI:
			regs[instr.rd()] = instr.U_imm();
			break;

		case Opcode::AUIPC:
			regs[instr.rd()] = last_pc + instr.U_imm();
			break;

		case Opcode::JAL: {
			auto link = pc;
			pc = last_pc + instr.J_imm();
			trap_check_pc_alignment();
			regs[instr.rd()] = link;
		} break;

		case Opcode::JALR: {
			auto link = pc;
			pc = (regs[instr.rs1()] + instr.I_imm()) & ~1;
			trap_check_pc_alignment();
			regs[instr.rd()] = link;
		} break;

		case Opcode::SB: {
			uint32_t addr = regs[instr.rs1()] + instr.S_imm();
			mem->store_byte(addr, regs[instr.rs2()]);
		} break;

		case Opcode::SH: {
			uint32_t addr = regs[instr.rs1()] + instr.S_imm();
			trap_check_addr_alignment<2, false>(addr);
			mem->store_half(addr, regs[instr.rs2()]);
		} break;

		case Opcode::SW: {
			uint32_t addr = regs[instr.rs1()] + instr.S_imm();
			trap_check_addr_alignment<4, false>(addr);
			mem->store_word(addr, regs[instr.rs2()]);
		} break;

		case Opcode::LB: {
			uint32_t addr = regs[instr.rs1()] + instr.I_imm();
			regs[instr.rd()] = mem->load_byte(addr);
		} break;

		case Opcode::LH: {
			uint32_t addr = regs[instr.rs1()] + instr.I_imm();
			trap_check_addr_alignment<2, true>(addr);
			regs[instr.rd()] = mem->load_half(addr);
		} break;

		case Opcode::LW: {
			uint32_t addr = regs[instr.rs1()] + instr.I_imm();
			trap_check_addr_alignment<4, true>(addr);
			regs[instr.rd()] = mem->load_word(addr);
		} break;

		case Opcode::LBU: {
			uint32_t addr = regs[instr.rs1()] + instr.I_imm();
			regs[instr.rd()] = mem->load_ubyte(addr);
		} break;

		case Opcode::LHU: {
			uint32_t addr = regs[instr.rs1()] + instr.I_imm();
			trap_check_addr_alignment<2, true>(addr);
			regs[instr.rd()] = mem->load_uhalf(addr);
		} break;

		case Opcode::BEQ:
			if (regs[instr.rs1()] == regs[instr.rs2()]) {
				pc = last_pc + instr.B_imm();
				trap_check_pc_alignment();
			}
			break;

		case Opcode::BNE:
			if (regs[instr.rs1()] != regs[instr.rs2()]) {
				pc = last_pc + instr.B_imm();
				trap_check_pc_alignment();
			}
			break;

		case Opcode::BLT:
			if (regs[instr.rs1()] < regs[instr.rs2()]) {
				pc = last_pc + instr.B_imm();
				trap_check_pc_alignment();
			}
			break;

		case Opcode::BGE:
			if (regs[instr.rs1()] >= regs[instr.rs2()]) {
				pc = last_pc + instr.B_imm();
				trap_check_pc_alignment();
			}
			break;

		case Opcode::BLTU:
			if ((uint32_t)regs[instr.rs1()] < (uint32_t)regs[instr.rs2()]) {
				pc = last_pc + instr.B_imm();
				trap_check_pc_alignment();
			}
			break;

		case Opcode::BGEU:
			if ((uint32_t)regs[instr.rs1()] >= (uint32_t)regs[instr.rs2()]) {
				pc = last_pc + instr.B_imm();
				trap_check_pc_alignment();
			}
			break;

		case Opcode::FENCE: {
			// TODO: check only IO fence, ignore MEM fence
			sc_core::wait(io_fence_event);
		} break;
		case Opcode::FENCE_I: {
			// not using out of order execution so can be ignored
		} break;

		case Opcode::ECALL: {
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
			// TODO: also raise trap and let the SW deal with it?
			status = CoreExecStatus::HitBreakpoint;
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
				}
				set_csr_value(addr, rs1_val);
			}
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
				if (rd != RegFile::zero)
					regs[rd] = csr_val;
				if (write)
					set_csr_value(addr, csr_val | rs1_val);
			}
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
				if (rd != RegFile::zero)
					regs[rd] = csr_val;
				if (write)
					set_csr_value(addr, csr_val & ~rs1_val);
			}
		} break;

		case Opcode::CSRRWI: {
			auto addr = instr.csr();
			if (is_invalid_csr_access(addr, true)) {
                RAISE_ILLEGAL_INSTRUCTION();
			} else {
				auto rd = instr.rd();
				if (rd != RegFile::zero) {
					regs[rd] = get_csr_value(addr);
				}
				set_csr_value(addr, instr.zimm());
			}
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
				if (rd != RegFile::zero)
					regs[rd] = csr_val;
				if (write)
					set_csr_value(addr, csr_val | zimm);
			}
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
				if (rd != RegFile::zero)
					regs[rd] = csr_val;
				if (write)
					set_csr_value(addr, csr_val & ~zimm);
			}
		} break;

		case Opcode::MUL: {
            REQUIRE_ISA(M_ISA_EXT);
			int64_t ans = (int64_t)regs[instr.rs1()] * (int64_t)regs[instr.rs2()];
			regs[instr.rd()] = ans & 0xFFFFFFFF;
		} break;

		case Opcode::MULH: {
            REQUIRE_ISA(M_ISA_EXT);
			int64_t ans = (int64_t)regs[instr.rs1()] * (int64_t)regs[instr.rs2()];
			regs[instr.rd()] = (ans & 0xFFFFFFFF00000000) >> 32;
		} break;

		case Opcode::MULHU: {
            REQUIRE_ISA(M_ISA_EXT);
			int64_t ans = ((uint64_t)(uint32_t)regs[instr.rs1()]) * (uint64_t)((uint32_t)regs[instr.rs2()]);
			regs[instr.rd()] = (ans & 0xFFFFFFFF00000000) >> 32;
		} break;

		case Opcode::MULHSU: {
            REQUIRE_ISA(M_ISA_EXT);
			int64_t ans = (int64_t)regs[instr.rs1()] * (uint64_t)((uint32_t)regs[instr.rs2()]);
			regs[instr.rd()] = (ans & 0xFFFFFFFF00000000) >> 32;
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
		} break;

		case Opcode::LR_W: {
            REQUIRE_ISA(A_ISA_EXT);
			uint32_t addr = regs[instr.rs1()];
			trap_check_addr_alignment<4, true>(addr);
			regs[instr.rd()] = mem->atomic_load_reserved_word(addr);
			if (lr_sc_counter == 0)
			    lr_sc_counter = 17;  // this instruction + 16 additional ones, (an over-approximation) to cover the RISC-V forward progress property
		} break;

		case Opcode::SC_W: {
            REQUIRE_ISA(A_ISA_EXT);
			uint32_t addr = regs[instr.rs1()];
			trap_check_addr_alignment<4, false>(addr);
			uint32_t val = regs[instr.rs2()];
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
			fp_finish_instr();
		} break;

		case Opcode::FCVT_WU_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			fp_setup_rm();
			regs[RD] = f32_to_ui32(fp_regs.f32(RS1), softfloat_roundingMode, true);
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
		} break;

		case Opcode::FEQ_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			regs[RD] = f32_eq(fp_regs.f32(RS1), fp_regs.f32(RS2));
			fp_update_exception_flags();
		} break;

		case Opcode::FLT_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			regs[RD] = f32_lt(fp_regs.f32(RS1), fp_regs.f32(RS2));
			fp_update_exception_flags();
		} break;

		case Opcode::FLE_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			regs[RD] = f32_le(fp_regs.f32(RS1), fp_regs.f32(RS2));
			fp_update_exception_flags();
		} break;

		case Opcode::FCLASS_S: {
            REQUIRE_ISA(F_ISA_EXT);
			fp_prepare_instr();
			regs[RD] = f32_classify(fp_regs.f32(RS1));
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
            fp_update_exception_flags();
        } break;

        case Opcode::FLT_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            regs[RD] = f64_lt(fp_regs.f64(RS1), fp_regs.f64(RS2));
            fp_update_exception_flags();
        } break;

        case Opcode::FLE_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            regs[RD] = f64_le(fp_regs.f64(RS1), fp_regs.f64(RS2));
            fp_update_exception_flags();
        } break;

        case Opcode::FCLASS_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            regs[RD] = (int64_t)f64_classify(fp_regs.f64(RS1));
        } break;

        case Opcode::FCVT_W_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            regs[RD] = f64_to_i32(fp_regs.f64(RS1), softfloat_roundingMode, true);
            fp_finish_instr();
        } break;

        case Opcode::FCVT_WU_D: {
            REQUIRE_ISA(D_ISA_EXT);
            fp_prepare_instr();
            fp_setup_rm();
            regs[RD] = (int32_t)f64_to_ui32(fp_regs.f64(RS1), softfloat_roundingMode, true);
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

            if (s_mode() && csrs.mstatus.tw)
                raise_trap(EXC_ILLEGAL_INSTR, instr.data());

            if (u_mode() && csrs.misa.has_supervisor_mode_extension())
                raise_trap(EXC_ILLEGAL_INSTR, instr.data());

            if (!ignore_wfi && !has_local_pending_enabled_interrupts())
                sc_core::wait(wfi_event);
            break;

        case Opcode::SFENCE_VMA:
            if (s_mode() && csrs.mstatus.tvm)
                raise_trap(EXC_ILLEGAL_INSTR, instr.data());
            mem->flush_tlb();
            break;

        case Opcode::URET:
            if (!csrs.misa.has_user_mode_extension())
                raise_trap(EXC_ILLEGAL_INSTR, instr.data());
            return_from_trap_handler(UserMode);
            break;

        case Opcode::SRET:
            if (!csrs.misa.has_supervisor_mode_extension() || (s_mode() && csrs.mstatus.tsr))
                raise_trap(EXC_ILLEGAL_INSTR, instr.data());
            return_from_trap_handler(SupervisorMode);
            break;

        case Opcode::MRET:
            return_from_trap_handler(MachineMode);
            break;

            // instructions accepted by decoder but not by this RV32IMACF ISS -> do normal trap
            // RV64I
        case Opcode::LWU:
        case Opcode::LD:
        case Opcode::SD:
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
		
			// Custom
		case Opcode::CUSTOM0:
		case Opcode::CUSTOM1:
		case Opcode::CUSTOM2:
		case Opcode::CUSTOM3:
			execute_rocc(instr);
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
			return csrs.time.low;
		}

		case TIMEH_ADDR:
		case MTIMEH_ADDR: {
			uint64_t mtime = clint->update_and_get_mtime();
			csrs.time.reg = mtime;
			return csrs.time.high;
		}

		case MCYCLE_ADDR:
			csrs.cycle.reg = _compute_and_get_current_cycles();
			return csrs.cycle.low;

		case MCYCLEH_ADDR:
			csrs.cycle.reg = _compute_and_get_current_cycles();
			return csrs.cycle.high;

		case MINSTRET_ADDR:
			return csrs.instret.low;

		case MINSTRETH_ADDR:
			return csrs.instret.high;

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
			if (csrs.mstatus.tvm)
				RAISE_ILLEGAL_INSTRUCTION();
			break;

		case FCSR_ADDR:
			return read(csrs.fcsr, FCSR_MASK);

		case FFLAGS_ADDR:
			return csrs.fcsr.fflags;

		case FRM_ADDR:
			return csrs.fcsr.frm;

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
            if (csrs.mstatus.tvm)
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
			csrs.fcsr.fflags = value;
			break;

		case FRM_ADDR:
			csrs.fcsr.frm = value;
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
	regs.write(idx, boost::lexical_cast<uint32_t>(value));
}

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
	csrs.mstatus.sd = 1;
	csrs.mstatus.fs = FS_DIRTY;
}

void ISS::fp_update_exception_flags() {
	if (softfloat_exceptionFlags) {
		fp_set_dirty();
		csrs.fcsr.fflags |= softfloat_exceptionFlags;
		softfloat_exceptionFlags = 0;
	}
}

void ISS::fp_setup_rm() {
	auto rm = instr.frm();
	if (rm == FRM_DYN)
		rm = csrs.fcsr.frm;
	if (rm >= FRM_RMM)
		RAISE_ILLEGAL_INSTRUCTION();
	softfloat_roundingMode = rm;
}

void ISS::fp_require_not_off() {
	if (csrs.mstatus.fs == FS_OFF)
		RAISE_ILLEGAL_INSTRUCTION();
}

void ISS::return_from_trap_handler(PrivilegeLevel return_mode) {
	switch (return_mode) {
		case MachineMode:
			prv = csrs.mstatus.mpp;
			csrs.mstatus.mie = csrs.mstatus.mpie;
			csrs.mstatus.mpie = 1;
			pc = csrs.mepc.reg;
			if (csrs.misa.has_user_mode_extension())
				csrs.mstatus.mpp = UserMode;
			else
				csrs.mstatus.mpp = MachineMode;
			break;

		case SupervisorMode:
			prv = csrs.mstatus.spp;
			csrs.mstatus.sie = csrs.mstatus.spie;
			csrs.mstatus.spie = 1;
			pc = csrs.sepc.reg;
			if (csrs.misa.has_user_mode_extension())
				csrs.mstatus.spp = UserMode;
			else
				csrs.mstatus.spp = SupervisorMode;
			break;

		case UserMode:
			prv = UserMode;
			csrs.mstatus.uie = csrs.mstatus.upie;
			csrs.mstatus.upie = 1;
			pc = csrs.uepc.reg;
			break;

		default:
			throw std::runtime_error("unknown privilege level " + std::to_string(return_mode));
	}

	if (trace)
		printf("[vp::iss] return from trap handler, time %s, pc %8x, prv %1x\n",
		       quantum_keeper.get_current_time().to_string().c_str(), pc, prv);
}

void ISS::trigger_external_interrupt(PrivilegeLevel level) {
	if (trace)
		std::cout << "[vp::iss] trigger external interrupt, " << sc_core::sc_time_stamp() << std::endl;

	switch (level) {
		case UserMode:
			csrs.mip.ueip = true;
			break;
		case SupervisorMode:
			csrs.mip.seip = true;
			break;
		case MachineMode:
			csrs.mip.meip = true;
			break;
	}

	wfi_event.notify(sc_core::SC_ZERO_TIME);
}

void ISS::clear_external_interrupt(PrivilegeLevel level) {
	if (trace)
		std::cout << "[vp::iss] clear external interrupt, " << sc_core::sc_time_stamp() << std::endl;

	switch (level) {
		case UserMode:
			csrs.mip.ueip = false;
			break;
		case SupervisorMode:
			csrs.mip.seip = false;
			break;
		case MachineMode:
			csrs.mip.meip = false;
			break;
	}
}

void ISS::trigger_timer_interrupt(bool status) {
	if (trace)
		std::cout << "[vp::iss] trigger timer interrupt=" << status << ", " << sc_core::sc_time_stamp() << std::endl;
	csrs.mip.mtip = status;
	wfi_event.notify(sc_core::SC_ZERO_TIME);
}

void ISS::trigger_software_interrupt(bool status) {
	if (trace)
		std::cout << "[vp::iss] trigger software interrupt=" << status << ", " << sc_core::sc_time_stamp() << std::endl;
	csrs.mip.msip = status;
	wfi_event.notify(sc_core::SC_ZERO_TIME);
}

void ISS::io_fence_done() {
	if (trace)
		std::cout << "[vp::iss] fence finished, " << sc_core::sc_time_stamp() << std::endl;
	io_fence_event.notify();
}

PrivilegeLevel ISS::prepare_trap(SimulationTrap &e) {
	// undo any potential pc update (for traps the pc should point to the originating instruction and not it's
	// successor)
	pc = last_pc;
	unsigned exc_bit = (1 << e.reason);

	// 1) machine mode execution takes any traps, independent of delegation setting
	// 2) non-delegated traps are processed in machine mode, independent of current execution mode
	if (prv == MachineMode || !(exc_bit & csrs.medeleg.reg)) {
		csrs.mcause.interrupt = 0;
		csrs.mcause.exception_code = e.reason;
		csrs.mtval.reg = boost::lexical_cast<uint32_t>(e.mtval);
		return MachineMode;
	}

	// see above machine mode comment
	if (prv == SupervisorMode || !(exc_bit & csrs.sedeleg.reg)) {
		csrs.scause.interrupt = 0;
		csrs.scause.exception_code = e.reason;
		csrs.stval.reg = boost::lexical_cast<uint32_t>(e.mtval);
		return SupervisorMode;
	}

	assert(prv == UserMode && (exc_bit & csrs.medeleg.reg) && (exc_bit & csrs.sedeleg.reg));
	csrs.ucause.interrupt = 0;
	csrs.ucause.exception_code = e.reason;
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
	if (x.meip)
		exc = EXC_M_EXTERNAL_INTERRUPT;
	else if (x.msip)
		exc = EXC_M_SOFTWARE_INTERRUPT;
	else if (x.mtip)
		exc = EXC_M_TIMER_INTERRUPT;
	else if (x.seip)
		exc = EXC_S_EXTERNAL_INTERRUPT;
	else if (x.ssip)
		exc = EXC_S_SOFTWARE_INTERRUPT;
	else if (x.stip)
		exc = EXC_S_TIMER_INTERRUPT;
	else if (x.ueip)
		exc = EXC_U_EXTERNAL_INTERRUPT;
	else if (x.usip)
		exc = EXC_U_SOFTWARE_INTERRUPT;
	else if (x.utip)
		exc = EXC_U_TIMER_INTERRUPT;
	else
		throw std::runtime_error("some pending interrupt must be available here");

	switch (e.target_mode) {
		case MachineMode:
			csrs.mcause.exception_code = exc;
			csrs.mcause.interrupt = 1;
			break;

		case SupervisorMode:
			csrs.scause.exception_code = exc;
			csrs.scause.interrupt = 1;
			break;

		case UserMode:
			csrs.ucause.exception_code = exc;
			csrs.ucause.interrupt = 1;
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
	if (m_pending && (prv < MachineMode || (prv == MachineMode && csrs.mstatus.mie))) {
		return {MachineMode, m_pending};
	}

	pending = pending & csrs.mideleg.reg;
	auto s_pending = pending & ~csrs.sideleg.reg;
	if (s_pending && (prv < SupervisorMode || (prv == SupervisorMode && csrs.mstatus.sie))) {
		return {SupervisorMode, s_pending};
	}

	auto u_pending = pending & csrs.sideleg.reg;
	if (u_pending && (prv == UserMode && csrs.mstatus.uie)) {
		return {UserMode, u_pending};
	}

	return {NoneMode, 0};
}

void ISS::switch_to_trap_handler(PrivilegeLevel target_mode) {
	if (trace) {
		printf("[vp::iss] switch to trap handler, time %s, last_pc %8x, pc %8x, irq %u, t-prv %1x\n",
		       quantum_keeper.get_current_time().to_string().c_str(), last_pc, pc, csrs.mcause.interrupt, target_mode);
	}

	// free any potential LR/SC bus lock before processing a trap/interrupt
	release_lr_sc_reservation();

	auto pp = prv;
	prv = target_mode;

	switch (target_mode) {
		case MachineMode:
			csrs.mepc.reg = pc;

			csrs.mstatus.mpie = csrs.mstatus.mie;
			csrs.mstatus.mie = 0;
			csrs.mstatus.mpp = pp;

			pc = csrs.mtvec.get_base_address();

			if (csrs.mcause.interrupt && csrs.mtvec.mode == csrs.mtvec.Vectored)
				pc += 4 * csrs.mcause.exception_code;
			break;

		case SupervisorMode:
			assert(prv == SupervisorMode || prv == UserMode);

			csrs.sepc.reg = pc;

			csrs.mstatus.spie = csrs.mstatus.sie;
			csrs.mstatus.sie = 0;
			csrs.mstatus.spp = pp;

			pc = csrs.stvec.get_base_address();

			if (csrs.scause.interrupt && csrs.stvec.mode == csrs.stvec.Vectored)
				pc += 4 * csrs.scause.exception_code;
			break;

		case UserMode:
			assert(prv == UserMode);

			csrs.uepc.reg = pc;

			csrs.mstatus.upie = csrs.mstatus.uie;
			csrs.mstatus.uie = 0;

			pc = csrs.utvec.get_base_address();

			if (csrs.ucause.interrupt && csrs.utvec.mode == csrs.utvec.Vectored)
				pc += 4 * csrs.ucause.exception_code;
			break;

		default:
			throw std::runtime_error("unknown privilege level " + std::to_string(target_mode));
	}
}

void ISS::performance_and_sync_update(Opcode::Mapping executed_op) {
    ++total_num_instr;

	if (!csrs.mcountinhibit.IR)
		++csrs.instret.reg;

	if (lr_sc_counter != 0) {
		--lr_sc_counter;
		assert (lr_sc_counter >= 0);
		if (lr_sc_counter == 0)
            release_lr_sc_reservation();
	}

	auto new_cycles = instr_cycles[executed_op];

	if (!csrs.mcountinhibit.CY)
		cycle_counter += new_cycles;

	quantum_keeper.inc(new_cycles);
	if (quantum_keeper.need_sync()) {
	    if (lr_sc_counter == 0) // match SystemC sync with bus unlocking in a tight LR_W/SC_W loop
		    quantum_keeper.sync();
	}
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
	do {
		run_step();
	} while (status == CoreExecStatus::Runnable);

	// force sync to make sure that no action is missed
	quantum_keeper.sync();
}

void ISS::show() {
	boost::io::ios_flags_saver ifs(std::cout);
	std::cout << "=[ core : " << csrs.mhartid.reg << " ]===========================" << std::endl;
	std::cout << "simulation time: " << sc_core::sc_time_stamp() << std::endl;
	regs.show();
	std::cout << "pc = " << std::hex << pc << std::endl;
	std::cout << "num-instr = " << std::dec << csrs.instret.reg << std::endl;
}
