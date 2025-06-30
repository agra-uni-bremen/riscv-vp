#pragma once

#include <tlm_utils/simple_target_socket.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>
#include <systemc>

struct MicroRV32UART : public sc_core::sc_module {
	tlm_utils::simple_target_socket<MicroRV32UART> tsock;

	sc_core::sc_event run_event;

	// tx
	char buf = 0;
	// rx
	std::queue<unsigned char> rxFifo;
	uint8_t rxFifoDepth = 16;

	SC_HAS_PROCESS(MicroRV32UART);

	MicroRV32UART(sc_core::sc_module_name) {
		tsock.register_b_transport(this, &MicroRV32UART::transport);
		SC_THREAD(run);
	}

	void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
		auto addr = trans.get_address();
		auto cmd = trans.get_command();
		// auto len = trans.get_data_length();
		auto ptr = trans.get_data_ptr(); // unsigned char

		std::cout << "[UART] addr: 0x" << std::hex << addr
          << ", cmd: " << (cmd == tlm::TLM_WRITE_COMMAND ? "WRITE" : "READ")
          << ", data_length: " << trans.get_data_length()
          << ", data: ";
		  for (unsigned i = 0; i < trans.get_data_length(); ++i) {
			std::cout << "0x" << std::hex << (int)((unsigned char)ptr[i]) << " ";
			}
			std::cout << std::endl;

		if (cmd == tlm::TLM_WRITE_COMMAND) {
			std::cout << "Write buf: " << buf <<  ", hex: 0x" << std::hex << (static_cast<int>(static_cast<unsigned char>(buf))) << std::endl;
			if (addr == 0) {
				buf = *ptr;
				std::cout << "cmd Write ptr: 0x" << std::hex << ptr << std::endl;
				std::cout << "cmd Write *ptr: 0x" << std::hex << *ptr << std::endl;
				std::cout << "cmd Write buf: " << buf << std::endl;
			} else if (addr == 4) {
				std::cout << buf << std::endl;;
			}
		} else if (cmd == tlm::TLM_READ_COMMAND) {
			std::cout << "rxFifo size: " << rxFifo.size() << std::endl;
			if (addr == 0) {
				// ignore
			} else if (addr == 4) {
				*((uint32_t *)ptr) = 1;
			} else if (addr == 8) {
				if (rxFifo.size() > 0) {
					*((uint32_t *)ptr) = rxFifo.front();
					printf("read RX: %2X\n", rxFifo.front());
					// printf("\n[TLM] UART, transport, read RX: %2X\n", rxFifo.front());
					// std::cout << std::endl << "[TLM] uart transport RX: " << std::hex <<
					// static_cast<uint8_t>(rxFifo.front()) << std::endl;
					rxFifo.pop();
				}
			} else if (addr == 12) {
				*((uint32_t *)ptr) = rxFifo.size();
			} else if (addr == 16) {
				*((uint32_t *)ptr) = rxFifo.size() == 1;
			}
		}
		// std::cout << "sc_time: " << sc_core::sc_time_stamp() << std::endl;
		// (void)delay;  // zero delay
		// wait(sc_core::sc_time(50, sc_core::SC_NS));
		delay += sc_core::sc_time(50, sc_core::SC_NS);
	}

	// untested
	void run() {
		while (true) {
			// 9600 baud ~= 1ms, 115200 ~= 87us
			// run_event.notify(sc_core::sc_time(1, sc_core::SC_MS));
			run_event.notify(sc_core::sc_time(87, sc_core::SC_US));
			sc_core::wait(run_event);  // 40 times per second by default
			unsigned char newRXChar = (rand() %('z'-'A')) + 'A';
			if (rxFifo.size() <= rxFifoDepth) {
				rxFifo.push(newRXChar);
				std::cout << "newRXChar: " << newRXChar << std::endl;
				// printf("\n[TLM] UART, run: %X\n", newRXChar);
				// std::cout << std::endl << "[TLM] uart run: " << std::hex << (newRXChar & 0xff) << std::endl;
			}
		}
	}
};
