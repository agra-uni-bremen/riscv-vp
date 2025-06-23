#pragma once

#include <tlm_utils/simple_target_socket.h>
#include <systemc>

#include "util/memory_map.h"

#include <fcntl.h>
#include <termios.h>
#include <deque>

struct UART16550 : public sc_core::sc_module {
	tlm_utils::simple_target_socket<UART16550> tsock;

	// memory mapped configuration registers
	RegisterRange mm_regs{0x0, 8};
	ArrayView<uint8_t> regs{mm_regs};

	std::vector<RegisterRange *> register_ranges{&mm_regs};

	std::deque<uint8_t> rx_fifo;
	bool initialized = false;

	static constexpr unsigned QUEUE_ADDR = 0;
	static constexpr unsigned LINESTAT_ADDR = 5;
	static constexpr unsigned STATUS_RX = 0x01;
	static constexpr unsigned STATUS_TX = 0x20;

	struct termios orig_termios;

	UART16550(sc_core::sc_module_name) {
		tsock.register_b_transport(this, &UART16550::transport);

		int flags = fcntl(0, F_GETFL, 0);
		fcntl(0, F_SETFL, flags | O_NONBLOCK);
		tcgetattr(STDIN_FILENO, &orig_termios);
		struct termios raw = orig_termios;
		raw.c_lflag &= ~(ICANON);  // Bytewise read
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

		mm_regs.pre_read_callback = std::bind(&UART16550::pre_read_regs, this, std::placeholders::_1);
		mm_regs.post_write_callback = std::bind(&UART16550::post_write_regs, this, std::placeholders::_1);
	}

	~UART16550() {
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
	}

	void try_receive_char() {
		if (rx_fifo.empty()) {
			char c;
			if (read(0, &c, 1) >= 0) {
				rx_fifo.push_back(c);
			}
		}
	}

	uint8_t take_next_char() {
		try_receive_char();
		if (rx_fifo.empty())
			return -1;

		uint8_t ans = rx_fifo.front();
		rx_fifo.pop_front();
		return ans;
	}

	bool pre_read_regs(RegisterRange::ReadInfo t) {
		if (t.addr == LINESTAT_ADDR) {
			regs[LINESTAT_ADDR] = 0;
			regs[LINESTAT_ADDR] |= STATUS_TX;
			try_receive_char();
			if (!rx_fifo.empty())
				regs[LINESTAT_ADDR] |= STATUS_RX;
		} else if (t.addr == QUEUE_ADDR) {
			regs[QUEUE_ADDR] = take_next_char();
		}

		return true;
	}

	void post_write_regs(RegisterRange::WriteInfo t) {
		if (t.addr == QUEUE_ADDR) {
			uint8_t data = *t.trans.get_data_ptr();
			if (!initialized) {
				initialized = true;
				if (data == 0)
					return;
			}
			std::cout << static_cast<char>(data);
			fflush(stdout);
		}
	}

	void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
		vp::mm::route("UART16550", register_ranges, trans, delay);
	}
};