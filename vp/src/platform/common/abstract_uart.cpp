#include "abstract_uart.h"

#include <err.h>
#include <fcntl.h>
#include <poll.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <mutex>
#include <queue>
#include <thread>

#define stop_fd (stop_pipe[0])
#define newpollfd(FD) \
	(struct pollfd){.fd = FD, .events = POLLIN | POLLERR, .revents = 0};

#define UART_TXWM (1 << 0)
#define UART_RXWM (1 << 1)
#define UART_FULL (1 << 31)

/* 8-entry transmit and receive FIFO buffers */
#define UART_FIFO_DEPTH 8

/* Extracts the interrupt trigger threshold from a control register */
#define UART_CTRL_CNT(REG) ((REG) >> 16)

enum {
	TXDATA_REG_ADDR = 0x0,
	RXDATA_REG_ADDR = 0x4,
	TXCTRL_REG_ADDR = 0x8,
	RXCTRL_REG_ADDR = 0xC,
	IE_REG_ADDR = 0x10,
	IP_REG_ADDR = 0x14,
	DIV_REG_ADDR = 0x18,
};

AbstractUART::AbstractUART(sc_core::sc_module_name, uint32_t irqsrc) {
	irq = irqsrc;
	tsock.register_b_transport(this, &AbstractUART::transport);

	router
	    .add_register_bank({
		{TXDATA_REG_ADDR, &txdata},
		{RXDATA_REG_ADDR, &rxdata},
		{TXCTRL_REG_ADDR, &txctrl},
		{RXCTRL_REG_ADDR, &rxctrl},
		{IE_REG_ADDR, &ie},
		{IP_REG_ADDR, &ip},
		{DIV_REG_ADDR, &div},
	    })
	    .register_handler(this, &AbstractUART::register_access_callback);

	stop = false;
	if (pipe(stop_pipe) == -1)
		throw std::system_error(errno, std::generic_category());

	sem_unlink("autx0");
	txfull_p = sem_open("autx0", O_CREAT | O_EXCL, 0700, 0);
	if (txfull_p == SEM_FAILED) {
		throw std::system_error(errno, std::generic_category());
	}
	sem_unlink("aurx0");
	rxempty_p = sem_open("aurx0",O_CREAT | O_EXCL, 0700, UART_FIFO_DEPTH);
	if (rxempty_p == SEM_FAILED) {
		throw std::system_error(errno, std::generic_category());
	}
	SC_METHOD(interrupt);
	sensitive << asyncEvent;
	dont_initialize();
}

AbstractUART::~AbstractUART(void) {
	stop = true;

	if (txthr) {
		spost(txfull_p); // unblock transmit thread
		txthr->join();
		delete txthr;
	}

	if (rcvthr) {
		uint8_t byte = 0;
		if (write(stop_pipe[1], &byte, sizeof(byte)) == -1) // unblock receive thread
			err(EXIT_FAILURE, "couldn't unblock uart receive thread");
		rcvthr->join();
		delete rcvthr;
	}

	close(stop_pipe[0]);
	close(stop_pipe[1]);

	sem_unlink("autx0");
	sem_close(txfull_p);
	sem_unlink("aurx0");
	sem_close(rxempty_p);
}

void AbstractUART::start_threads(int fd) {
	fds[0] = newpollfd(stop_fd);
	fds[1] = newpollfd(fd);

	rcvthr = new std::thread(&AbstractUART::receive, this);
	txthr = new std::thread(&AbstractUART::transmit, this);
}

void AbstractUART::rxpush(uint8_t data) {
	swait(rxempty_p);

	rcvmtx.lock();
	rx_fifo.push(data);
	rcvmtx.unlock();
	asyncEvent.notify();
}

void AbstractUART::register_access_callback(const vp::map::register_access_t &r) {
	if (r.read) {
		if (r.vptr == &txdata) {
			txmtx.lock();
			txdata = (tx_fifo.size() >= UART_FIFO_DEPTH) ? UART_FULL : 0;
			txmtx.unlock();
		} else if (r.vptr == &rxdata) {
			rcvmtx.lock();
			if (rx_fifo.empty()) {
				rxdata = 1 << 31;
			} else {
				rxdata = rx_fifo.front();
				rx_fifo.pop();

				spost(rxempty_p);
			}

			rcvmtx.unlock();
		} else if (r.vptr == &txctrl) {
			// std::cout << "TXctl";
		} else if (r.vptr == &rxctrl) {
			// std::cout << "RXctrl";
		} else if (r.vptr == &ip) {
			uint32_t ret = 0;
			txmtx.lock();
			if (tx_fifo.size() < UART_CTRL_CNT(txctrl)) {
				ret |= UART_TXWM;
			}
			txmtx.unlock();
			rcvmtx.lock();
			if (rx_fifo.size() > UART_CTRL_CNT(rxctrl)) {
				ret |= UART_RXWM;
			}
			rcvmtx.unlock();
			ip = ret;
		} else if (r.vptr == &ie) {
			// do nothing
		} else if (r.vptr == &div) {
			// just return the last set value
		} else {
			std::cerr << "invalid offset for UART " << std::endl;
		}
	}

	bool notify = false;
	if (r.write) {
		if (r.vptr == &txctrl && UART_CTRL_CNT(r.nv) < UART_CTRL_CNT(txctrl))
			notify = true;
		else if (r.vptr == &rxctrl && UART_CTRL_CNT(r.nv) < UART_CTRL_CNT(rxctrl))
			notify = true;
	}

	r.fn();

	if (notify || (r.write && r.vptr == &ie))
		asyncEvent.notify();

	if (r.write && r.vptr == &txdata) {
		txmtx.lock();
		if (tx_fifo.size() >= UART_FIFO_DEPTH) {
			txmtx.unlock();
			return; /* write is ignored */
		}

		tx_fifo.push(txdata);
		txmtx.unlock();
		spost(txfull_p);
	}
}

void AbstractUART::transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
	router.transport(trans, delay);
}

void AbstractUART::transmit(void) {
	uint8_t data;

	while (!stop) {
		swait(txfull_p);
		if (stop) break;

		txmtx.lock();
		data = tx_fifo.front();
		tx_fifo.pop();
		txmtx.unlock();

		asyncEvent.notify();
		write_data(data);
	}
}

void AbstractUART::receive(void) {
	while (!stop) {
		if (poll(fds, (nfds_t)NFDS, -1) == -1)
			throw std::system_error(errno, std::generic_category());

		/* stop_fd is checked first as it is fds[0] */
		for (size_t i = 0; i < NFDS; i++) {
			int fd = fds[i].fd;
			short ev = fds[i].revents;

			if (ev & POLLERR) {
				throw std::runtime_error("received unexpected POLLERR");
			} else if (ev & POLLIN) {
				if (fd == stop_fd)
					break;
				else
					handle_input(fd);
			}
		}
	}
}

void AbstractUART::interrupt(void) {
	bool trigger = false;

	/* XXX: Possible optimization would be to trigger the
	 * interrupt from the background thread. However,
	 * the PLIC methods are very likely not thread safe. */

	if (ie & UART_RXWM) {
		rcvmtx.lock();
		if (rx_fifo.size() > UART_CTRL_CNT(rxctrl))
			trigger = true;
		rcvmtx.unlock();
	}

	if (ie & UART_TXWM) {
		txmtx.lock();
		if (tx_fifo.size() < UART_CTRL_CNT(txctrl))
			trigger = true;
		txmtx.unlock();
	}

	if (trigger)
		plic->gateway_trigger_interrupt(irq);
}

void AbstractUART::swait(sem_t *sem) {
	if (sem_wait(sem))
		throw std::system_error(errno, std::generic_category());
}

void AbstractUART::spost(sem_t *sem) {
	if (sem_post(sem))
		throw std::system_error(errno, std::generic_category());
}
	