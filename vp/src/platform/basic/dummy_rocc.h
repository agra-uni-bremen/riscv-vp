#pragma once

#include <tlm_utils/peq_with_get.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include <array>
#include <systemc>

#include "core/common/io_fence_if.h"
#include "core/common/rocc_if.h"
#include "core/common/trap.h"

class DummyRocc : public rocc_if, public sc_core::sc_module {
   public:
	std::array<tlm_utils::simple_initiator_socket<DummyRocc>, 2> isocks{};
	std::array<tlm_utils::simple_target_socket<DummyRocc>, 2> tsocks{};
	tlm_utils::peq_with_get<tlm::tlm_generic_payload> peq;
	io_fence_if& core;

	SC_HAS_PROCESS(DummyRocc);

	DummyRocc(sc_core::sc_module_name name, io_fence_if& core) : 
		sc_core::sc_module(name), peq("dummy_rocc_peq"), core(core) {
		memset(acc, 0, sizeof(acc));
		tsocks[0].register_b_transport(this, &DummyRocc::transport_core);
		// TODO: memory has no controller (sc_thread), no concurrent memory access from both CORE & ROCC
		tsocks[1].register_b_transport(this, &DummyRocc::transport_mem);
		SC_THREAD(run);
	}

	void transport_core(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
		auto addr = trans.get_address();
		assert(addr >= ROCC_START_ADDR && addr <= ROCC_END_ADDR);
		sc_core::wait(delay);
		peq.notify(trans);
	}

	void transport_mem(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
		assert(false);
	}

	void run() {
		while (true) {
			sc_core::wait(peq.get_event());
			tlm::tlm_generic_payload* trans = peq.get_next_transaction();
			while (trans) {
				auto req = (RoccCmd*)trans->get_data_ptr();
				auto instr = req->instr;
				if (instr.rs2() >= num_acc)
					raise_trap(EXC_ILLEGAL_INSTR, instr.data());

				reg_t prev_acc = acc[instr.rs2()];
				
				switch (instr.funct7()) {
					case 0:  // acc <- xs1
						assert(instr.xs1() && instr.xs2());
						acc[instr.rs2()] = req->rs1;
						break;
					case 1:  // xd <- acc (the only real work is the return statement below)
						assert(instr.xd());
						break;
					case 2: {  // acc[rs2] <- Mem[xs1]
						assert(instr.xs1() && instr.xs2());
						tlm::tlm_generic_payload mem_trans;
						mem_trans.set_command(tlm::TLM_READ_COMMAND);
						mem_trans.set_address(req->rs1);
						mem_trans.set_data_ptr((unsigned char*)&acc[instr.rs2()]);
						mem_trans.set_data_length(sizeof(reg_t));
						mem_trans.set_response_status(tlm::TLM_OK_RESPONSE);
						auto zero_delay = sc_core::SC_ZERO_TIME;
						isocks[1]->b_transport(mem_trans, zero_delay);

						if (mem_trans.is_response_error()) {
							if (core.traced())
								std::cout << "WARNING: Rocc memory transaction failed -> raise trap" << std::endl;
							raise_trap(EXC_LOAD_PAGE_FAULT, req->rs1);
						}
					} break;
					case 3:  // acc[rs2] <- accX + xs1
						assert(instr.xs1() && instr.xs2());
						acc[instr.rs2()] += req->rs1;
						break;
					default:
						raise_trap(EXC_ILLEGAL_INSTR, instr.data());

						// in all cases, xd <- previous value of acc[rs2]
						if (instr.xd()) {
							RoccResp resp;
							resp.rd = instr.rd();
							resp.data = prev_acc;
							tlm::tlm_generic_payload resp_trans;
							resp_trans.set_command(tlm::TLM_IGNORE_COMMAND);
							resp_trans.set_address(ROCC_START_ADDR);
							resp_trans.set_data_ptr((unsigned char*)&resp_trans);
							resp_trans.set_data_length(sizeof(resp));
							resp_trans.set_response_status(tlm::TLM_OK_RESPONSE);
							auto zero_delay = sc_core::SC_ZERO_TIME;
							isocks[0]->b_transport(resp_trans, zero_delay);
						}
				}
				// TODO: profile different op costs
				wait(10, sc_core::SC_NS);
				trans = peq.get_next_transaction();
			}
			// peq flushed, notify the core to continue if fenced on io
			core.io_fence_done();
		}
	}

   private:
	static const int num_acc = 4;
	reg_t acc[num_acc];
};