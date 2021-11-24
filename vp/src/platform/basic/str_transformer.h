/**
 * @file str_transformer.h
 * @author cw (wei.c.chen@intel.com)
 * @brief
 * @version 0.1
 * @date 2021-11-23
 *
 * Copyright (C) 2020, Intel Corporation. All rights reserved.
 *
 * A string transformer as RoCC accelerator. It has two sets of resources for buffering.
 *
 *   ---------------------------------------------------
 * 	| funct7 | rs2 | rs1 | xd | xs1 | xs2 | rd | opcode |
 *   ---------------------------------------------------
 *
 * 	2 operations are supported with different funct7() values:
 * 	0. Register configuration
 *   ----------------------------------------------
 * 	| 0 | rs2 | rs1 | xd | xs1 | xs2 | rd | opcode |
 *   ----------------------------------------------
 *  regs[rs2]: the index of the target register to be configured
 *  regs[rs1]: the value of the configured value
 *
 *  1. Kick off a buffer
 *   ----------------------------------------------
 * 	| 1 | rs2 | rs1 | xd | xs1 | xs2 | rd | opcode |
 *   ----------------------------------------------
 *  regs[rs2]: the index of the buffer to be kicked off
 */

#pragma once

#include <tlm_utils/peq_with_get.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include <array>
#include <memory>
#include <systemc>

#include "allocator.h"
#include "core/common/io_fence_if.h"
#include "core/common/rocc_if.h"
#include "core/common/trap.h"
#include "mem.h"

using std::shared_ptr;

#ifdef RV32B
using rv32::GenericMemoryProxy;
#else
using rv64::GenericMemoryProxy;
#endif

class StrTransformer : public rocc_if, public sc_core::sc_module {
   public:
	std::array<tlm_utils::simple_initiator_socket<StrTransformer>, 1> isocks{};
	GenericMemoryProxy<reg_t>* mem_if;
	std::array<tlm_utils::simple_target_socket<StrTransformer>, 2> tsocks{};
	tlm_utils::peq_with_get<tlm::tlm_generic_payload> peq;
	io_fence_if& core;
	tlm_utils::tlm_quantumkeeper quantum_keeper;
	TransAllocator<Transaction<RoccResp>> trans_allocator;

	SC_HAS_PROCESS(StrTransformer);

	StrTransformer(sc_core::sc_module_name name, io_fence_if& core)
	    : sc_core::sc_module(name), peq("str_transformer_peq"), core(core) {
		for (int i = 0; i < num_buffers; i++) {
			memset(buffer[i], 0, sizeof(buffer[i]));
		}

		sc_core::sc_time ins_time(10, sc_core::SC_NS);
		for (int i = 0; i < num_funcs; i++) {
			instr_cycles[i] = ins_time;
		}

		tsocks[0].register_b_transport(this, &StrTransformer::transport_core);
		// TODO: memory has no controller (sc_thread), no concurrent memory access from both CORE & ROCC
		tsocks[1].register_b_transport(this, &StrTransformer::transport_mem);
		SC_THREAD(run);
	}

	void init(GenericMemoryProxy<reg_t>* mem_proxy) {
		mem_if = mem_proxy;
	}

	bool is_busy() const override {
		for (auto& p : phases)
			if (p == TransPhase::RUNNING)
				return true;
		return false;
	}

	void transport_core(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
		auto addr = trans.get_address();
		assert(addr >= ROCC_START_ADDR && addr <= ROCC_END_ADDR);
		peq.notify(trans);
	}

	void transport_mem(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
		assert(false);
	}

	void run() {
		for (int i = 0; i < num_buffers; i++) {
			sc_core::sc_spawn(sc_bind(&StrTransformer::transform, this, i));
		}

		while (true) {
			sc_core::wait(peq.get_event());
			tlm::tlm_generic_payload* trans = peq.get_next_transaction();
			while (trans) {
				quantum_keeper.inc(contr_cycle);
				if (quantum_keeper.need_sync()) {
					quantum_keeper.sync();
				}

				auto req = (RoccCmd*)trans->get_data_ptr();
				auto& instr = req->instr;
				auto func = instr.funct7();
				switch (func) {
					case 0: {  // configure
						assert(instr.xs1() && instr.xs2() && !instr.xd());
						auto& index = req->rs2;
						auto& value = req->rs1;
						auto buffer_index = index / 10;
						auto local_index = index % 10;
						assert(buffer_index < num_buffers);
						assert(local_index < 4);
						assert(phases[buffer_index] <= TransPhase::CONFIG);
						if (local_index == 0)
							src_addr[buffer_index] = value;
						else if (local_index == 1)
							dst_addr[buffer_index] = value;
						else if (local_index == 2)
							str_size[buffer_index] = value;
						else
							buffer_func[buffer_index] = value;
						phases[buffer_index] = TransPhase::CONFIG;
						break;
					}
					case 1: {  // run
						assert(!instr.xs1() && instr.xs2() && !instr.xd());
						auto& buffer_index = req->rs2;
						assert(buffer_index < num_buffers);
						assert(phases[buffer_index] <= TransPhase::CONFIG);
						phases[buffer_index] = TransPhase::RUNNING;
						run_event[buffer_index].notify();
						break;
					}
					default:
						raise_trap(EXC_ILLEGAL_INSTR, instr.data());
				}

				trans->release();
				trans = peq.get_next_transaction();
			}
		}
	}

   private:

    void to_lower(char* buffer, reg_t len) {
		char* c = buffer;
		static int inc = 'a' - 'A';
		while (len > 0) {
			if (*c >= 'A' && *c <= 'Z')
				*c += inc;
			c++;
			len--;
		}
	}

	void to_upper(char* buffer, reg_t len) {
		char* c = buffer;
		static int inc = 'a' - 'A';
		while (len > 0) {
			if (*c >= 'a' && *c <= 'z')
				*c -= inc;
			c++;
			len--;
		}
	}

	void load_data(int buffer_index) {
		auto len = str_size[buffer_index];
		auto addr = src_addr[buffer_index];
		auto offset = 0;
		reg_t data;
		while (len > sizeof(reg_t)) {
			data = mem_if->load(addr);
			memcpy(buffer[buffer_index] + offset, &data, sizeof(reg_t));
			offset += sizeof(reg_t);
			len -= sizeof(reg_t);
			addr += sizeof(reg_t);
		}
		if (len > 0) {
			data = mem_if->load(addr);
			memcpy(buffer[buffer_index] + offset, &data, len);
		}
	}

	void store_data(int buffer_index) {
		auto len = str_size[buffer_index];
		auto addr = dst_addr[buffer_index];
		auto offset = 0;
		reg_t data;
		while (len > sizeof(reg_t)) {
			memcpy(&data, buffer[buffer_index] + offset, sizeof(reg_t));
			mem_if->store(addr, data);
			offset += sizeof(reg_t);
			len -= sizeof(reg_t);
			addr += sizeof(reg_t);
		}
		if (len > 0) {
			memcpy(&data, buffer[buffer_index] + offset, len);
			mem_if->store(addr, data);
		}
	}

	void transform(int buffer_index) {
		while (true) {
			wait(run_event[buffer_index]);
			assert(buffer_func[buffer_index] < num_funcs);
			wait(instr_cycles[buffer_func[buffer_index]]);
			switch (buffer_func[buffer_index]) {
				case TransOp::TO_LOWWER: {
					load_data(buffer_index);
					to_lower(buffer[buffer_index], str_size[buffer_index]);
					store_data(buffer_index);
					break;
				}
				case TransOp::TO_UPPER: {
					load_data(buffer_index);
					to_upper(buffer[buffer_index], str_size[buffer_index]);
					store_data(buffer_index);
					break;
				}
				default:
					raise_trap(EXC_ILLEGAL_INSTR, buffer_func[buffer_index]);
			}
			
			phases[buffer_index] = TransPhase::IDLE;
			if (!is_busy()) {
				core.io_fence_done();
			}
		}
	}

   private:
	static constexpr int num_buffers = 2;
	static constexpr int num_funcs = 2;
	static constexpr int buffer_size = 256;
	static const sc_core::sc_time contr_cycle;

	enum TransOp { TO_LOWWER = 0, TO_UPPER };

	enum TransPhase { IDLE = 0, CONFIG, RUNNING } phases[num_buffers]{TransPhase::IDLE};

	// register address map
	// [0,  1,  2,  3]  -> [src_addr[0], dst_addr[0], str_size[0], buffer_func[0]]
	// [10, 11, 12, 13] -> [src_addr[1], dst_addr[1], str_size[1], buffer_func[1]]
	// ...
	reg_t src_addr[num_buffers]{0};
	reg_t dst_addr[num_buffers]{0};
	reg_t str_size[num_buffers]{0};
	reg_t buffer_func[num_buffers]{0};

	char buffer[num_buffers][buffer_size];
	std::array<sc_core::sc_time, num_funcs> instr_cycles;
	sc_core::sc_event run_event[num_buffers];
};

const sc_core::sc_time StrTransformer::contr_cycle(10, sc_core::SC_NS);