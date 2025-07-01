#pragma once

#include <tlm_utils/simple_target_socket.h>
#include "core/common/irq_if.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>
#include <systemc>

// UART registers
#define THR 0  // Transmit Holding Register
#define RHR 0 // Receiver Holding Register
#define IER 1  // Interrupt Enable Register
#define ISR 2  // Interrupt Identification Register
#define FCR 2  // FIFO Control Register (write-only)
#define LCR 3  // Line Control Register
#define MCR 4  // Modem Control Register
#define LSR 5  // Line Status Register
#define MSR 6  // Modem Status Register
#define SPR 7  // Scratch Register

// Control bits
#define UART_LCR_DLAB 0x80  // Divisor Latch Access Bit
#define UART_LSR_THRE 0x20  // Transmit Holding Register Empty
#define UART_LSR_TEMT 0x40 // Transmitter empty
#define UART_LSR_DR   0x01  // Data Ready
#define UART_LSR_BI   0x10	// Break interrupt indicator
#define UART_LSR_OE   0x02	/* Overrun error indicator */
#define UART_FCR_FE   0x01 // FIFO enable
#define UART_FCR_XFR  0x04    /* XMIT Fifo Reset */
#define UART_FCR_RFR  0x02    /* RCVR Fifo Reset */
#define UART_ISR_FE   0xC0    /* Fifo enabled */
#define UART_ISR_ID   0x06	/* Mask for the interrupt ID */
#define UART_ISR_THRI 0x02	/* Transmitter holding register empty */
#define UART_MCR_LOOP 0x10	/* Enable loopback test mode */
#define UART_MSR_ANY_DELTA 0x0F	/* Any of the delta bits! */
#define UART_LSR_INT_ANY 0x1E	/* Any of the lsr-interrupt-triggering status bits */
#define UART_ISR_MSI	0x00	/* Modem status interrupt */
#define UART_ISR_THRI	0x02	/* Transmitter holding register empty */
#define UART_ISR_RDI	0x04	/* Receiver data interrupt */
#define UART_ISR_RLSI	0x06	/* Receiver line status interrupt */
#define UART_ISR_CTI    0x0C    /* Character Timeout Indication */
#define UART_IER_MSI  0x08	/* Enable Modem status interrupt */
#define UART_IER_RLSI 0x04	/* Enable receiver line status interrupt */
#define UART_IER_THRI 0x02	/* Enable Transmitter holding register int. */
#define UART_IER_RDI  0x01	/* Enable receiver data interrupt */

uint8_t DLL = 0;  // Divisor Latch LSB (when DLAB=1)
uint8_t DLM = 0;  // Divisor Latch MSB (when DLAB=1)
uint8_t THR_reg = 0;
uint8_t FCR_reg = 0;
uint8_t IER_reg = 0;
uint8_t ISR_reg = 0;
uint8_t LCR_reg = 0;
uint8_t MCR_reg = 0;
uint8_t LSR_reg = UART_LSR_THRE;  // Initially THR is empty
uint8_t MSR_reg = 0;
uint8_t SPR_reg = 0;
uint8_t RHR_reg = 0;
uint8_t ISR_NO_INT = 0x01; // Interrupt identification register

/* Interrupt trigger levels. The byte-counts are for 16550A - in newer UARTs the byte-count for each ITL is higher. */

#define UART_FCR_ITL_1      0x00 /* 1 byte ITL */
#define UART_FCR_ITL_2      0x40 /* 4 bytes ITL */
#define UART_FCR_ITL_3      0x80 /* 8 bytes ITL */
#define UART_FCR_ITL_4      0xC0 /* 14 bytes ITL */

struct Qemu32UART : public sc_core::sc_module {
	tlm_utils::simple_target_socket<Qemu32UART> tsock;

	sc_core::sc_event run_event;

	// bool trigger_send = false;  // Flag to trigger transmission
	interrupt_gateway *plic = 0;
	uint32_t irq_number = 0;
	int poll_msl;
	int thr_ipending;
	int timeout_ipending;           // timeout interrupt pending state
	uint8_t recv_fifo_itl;          // Interrupt trigger level for recv_fifo

	// tx
	char tx_buffer = 0;
	std::queue<unsigned char> txFifo;
	// rx
	std::queue<unsigned char> rxFifo;
	uint8_t rxFifoDepth = 16;

	SC_HAS_PROCESS(Qemu32UART);

	Qemu32UART(sc_core::sc_module_name, uint32_t irq_number) : irq_number(irq_number) {
		tsock.register_b_transport(this, &Qemu32UART::transport);
		SC_THREAD(run);
	}
	void check_interrupts() {
                uint8_t new_isr = ISR_NO_INT;

		if ((IER_reg & UART_IER_RLSI) && (LSR_reg & UART_LSR_INT_ANY)) {  // Reciever Line Status (Prio 1)
			new_isr = UART_ISR_RLSI;
		} else if ((IER_reg & UART_IER_RDI) && (LSR_reg & UART_LSR_DR)) {  // Received Data Ready (Prio 2)
                        new_isr = UART_ISR_RDI; 
		} else if ((IER_reg & UART_IER_RDI) && (timeout_ipending)) {  // Reception Timeout (Prio 2)
                        new_isr = UART_ISR_CTI;
		} else if ((IER_reg & UART_IER_THRI) && (thr_ipending)) {  // Transmitter Holding Register Empty (Prio 3)
                        new_isr = UART_ISR_THRI;
		} else if ((IER_reg & UART_IER_MSI) && (MSR_reg & UART_MSR_ANY_DELTA)) {  // Modem Status (Prio 4)
                        new_isr = UART_ISR_MSI;
                }

                ISR_reg = new_isr | (ISR_reg & 0xF0); // Preserve FIFO and DMA flags

                if (new_isr != ISR_NO_INT) {
                    plic->gateway_trigger_interrupt(new_isr);
                } else {
                    plic->gateway_trigger_interrupt(ISR_NO_INT); // Default
                }
	}

	void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
		auto addr = trans.get_address();
		auto cmd = trans.get_command();
		auto ptr = trans.get_data_ptr();
		uint32_t *data_ptr = reinterpret_cast<uint32_t*>(ptr);

		addr &= 0x7;  // 8 registers (0-7)

		if (cmd == tlm::TLM_WRITE_COMMAND) {
			char value = *ptr;
			std::cout << "[UART WRITE] addr: 0x" << std::hex << (int)addr
			          << ", value: " << value << std::endl;

			switch (addr) {
				case THR:  // Write to THR or DLL (if DLAB=1)
					if ((LSR_reg & UART_LSR_THRE) && !(LCR_reg & UART_LCR_DLAB)) { // check that THR Empty flag is set in the LSR
						// trigger_send = true;
						THR_reg = value;
						if(FCR_reg & UART_FCR_FE) {
							if (txFifo.size() == rxFifoDepth) {
								// If the FIFO is full, this pops the oldest byte from the FIFO.
								txFifo.pop();
							}
							txFifo.push(value); // Store character
						}
						thr_ipending = 0;
						LSR_reg &= ~UART_LSR_THRE;  // THR not empty, transmitter busy
						LSR_reg &= ~UART_LSR_TEMT; // Tx not empty
						// Check if the transmitter is busy
						// if (LSR_reg & UART_LSR_TEMT) {}
						// irq
                                                check_interrupts();
						run_event.notify(); // call run
					}
					break;

				case IER:
				{
					if (!(LCR_reg & UART_LCR_DLAB)) {
						uint8_t changed = (IER_reg ^ value) & 0x0f;
						IER_reg = value & 0x0f;

						if (changed & UART_IER_THRI) {
							if ((IER_reg & UART_IER_THRI) && (LCR_reg & UART_LSR_THRE)) {
								thr_ipending = 1;
							} else {
								thr_ipending = 0;
							}
						}

						if (changed) {
                                                        check_interrupts();
							// serial_update_irq(s);
						}
					}
				}
				break;

				case FCR:
					if ((value ^ FCR_reg) & UART_FCR_FE) { // FIFO enable/disable flag change
						value |= UART_FCR_XFR | UART_FCR_RFR;
					}

					// check timeout, FIFO clear
					if (value & UART_FCR_RFR) { // check if FIFO receive clear flag is set
						LSR_reg &= ~(UART_LSR_DR | UART_LSR_BI);
						timeout_ipending = 0;
						rxFifo = std::queue<unsigned char>(); // reset Rx FIFO
					}

					if (value & UART_FCR_XFR) {
						LSR_reg |= UART_LSR_THRE;
						thr_ipending = 1;
						txFifo = std::queue<unsigned char>();
					}

					/* serial write fcr */
					FCR_reg = value;
					if (value & UART_FCR_FE) {
						ISR_reg |= UART_ISR_FE;
						/* Set recv_fifo trigger Level */
						switch (value & 0xC0) {
							case UART_FCR_ITL_1:
								recv_fifo_itl = 1;
								break;
							case UART_FCR_ITL_2:
								recv_fifo_itl = 4;
								break;
							case UART_FCR_ITL_3:
								recv_fifo_itl = 8;
								break;
							case UART_FCR_ITL_4:
								recv_fifo_itl = 14;
								break;
						}
					} else {
						ISR_reg &= ~UART_ISR_FE;
					}
                                        check_interrupts();
					// serial_update_irq(s);
					break;

				case LCR:
				{
					LCR_reg = value;
					//  serial_update_parameters(s); // update LCR
				}
					break;

				case MCR:
					MCR_reg = value & 0x1f;
					// update timer
					break;

				case LSR:  // Read-only
					break;

				case MSR:  // Read-only
					break;

				case SPR:
					SPR_reg = value;
					break;
			}
		} else if (cmd == tlm::TLM_READ_COMMAND) {
			uint32_t ret;
			char value = *ptr;
			std::cout << "[UART READ] addr: 0x" << std::hex << (int)addr
			          << ", value: " << value << std::endl;

			switch (addr) {
				case RHR:  // Read from RBR or DLL
					if ((LSR_reg & UART_LSR_THRE) && !(LCR_reg & UART_LCR_DLAB)) {
						if(FCR_reg & UART_FCR_FE) {
							if (!rxFifo.empty()) {
								ret = rxFifo.front();
								rxFifo.pop();
							} else {
								ret = 0;
							}

							if (rxFifo.size() == 0) {
								LSR_reg &= ~(UART_LSR_DR | UART_LSR_BI);
							} // else: update timer
						} else {
							ret = RHR_reg;
							LSR_reg &= ~(UART_LSR_DR | UART_LSR_BI);
						}
                                                check_interrupts();
						// serial_update_irq(s);
						// if (!(MCR_reg & UART_MCR_LOOP)) {}
					}
					break;

				case IER:
					ret = IER_reg;
					break;

				case ISR:
					ret = ISR_reg;
					if ((ret & UART_ISR_ID) == UART_ISR_THRI) {
						thr_ipending = 0;
                                                check_interrupts();
						// serial_update_irq(s);
					}
					break;

				case LCR:
					ret = LCR_reg;
					break;

				case MCR:
					ret = MCR_reg;
					break;

				case LSR:
					ret = LSR_reg;
					/* Clear break and overrun interrupts */
					if (LSR_reg & (UART_LSR_BI|UART_LSR_OE)) {
						LSR_reg &= ~(UART_LSR_BI|UART_LSR_OE);
                                                check_interrupts();
						// serial_update_irq(s);
					}
					break;

				case MSR:
					if (MCR_reg & UART_MCR_LOOP) {
						/* in loopback, the modem output pins are connected to the
						   inputs */
						ret = (MCR_reg & 0x0c) << 4;
						ret |= (MCR_reg & 0x02) << 3;
						ret |= (MCR_reg & 0x01) << 5;
					} else {
						if (poll_msl >= 0)
							// serial_update_msl(s);
						ret = MSR_reg;
						/* Clear delta bits & msr int after read, if they were set */
						if (MSR_reg & UART_MSR_ANY_DELTA) {
							MSR_reg &= 0xF0;
                                                        check_interrupts();
							// serial_update_irq(s);
						}
					}
					break;

				case SPR:
					ret = SPR_reg;
					break;
			}
		}

		delay += sc_core::sc_time(50, sc_core::SC_NS);
	}


//	sc_core::sc_time calculateTransmissionDelay() {
//		// Calculate based on baud rate (DLL/DLM)
//		// Default to 115200 baud if divisor is 0
//		uint16_t divisor = (DLM << 8) | DLL;
//		if (divisor == 0) divisor = 1;
//		double baud_rate = 115200.0 / divisor;
//		double bit_time = 1.0 / baud_rate;
//		return sc_core::sc_time(bit_time * 10, sc_core::SC_SEC);  // 10 bits per char (8N1)
//	}

	void run() {
		while (true) {
			// check interrupts

			// Check if there are characters to transmit
			if (thr_ipending) {
				// Transmit data
				char tx_char = txFifo.front();
				std::cout << "tx_char: " << tx_char << std::endl;
				std::cout << tx_char << std::flush;  // Actual output
				txFifo.pop();
				THR_reg = tx_char;
				LSR_reg &= ~UART_LSR_TEMT;
				run_event.notify(sc_core::sc_time(87, sc_core::SC_US));
			}

			if (rxFifo.size() > 0) {
				// Get the next character from the receive FIFO
				char rx_char = rxFifo.front();
				rxFifo.pop();
				RHR_reg = rx_char;
				LSR_reg |= UART_LSR_DR;
			}
			sc_core::wait(run_event);
		}
	}
};
