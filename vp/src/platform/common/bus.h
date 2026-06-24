#ifndef RISCV_ISA_BUS_H
#define RISCV_ISA_BUS_H

#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <systemc>

#include <map>
#include <stdexcept>
#include <memory>

struct PortMapping {
	uint64_t start;
	uint64_t end;

	PortMapping(uint64_t start, uint64_t end) : start(start), end(end) {
		assert(end >= start);
	}

	bool contains(uint64_t addr) {
		return addr >= start && addr <= end;
	}

	uint64_t global_to_local(uint64_t addr) {
		return addr - start;
	}
};

template <unsigned int NR_OF_INITIATORS, unsigned int NR_OF_TARGETS>
struct SimpleBus : sc_core::sc_module {
	std::array<tlm_utils::simple_target_socket<SimpleBus>, NR_OF_INITIATORS> tsocks;

	std::array<tlm_utils::simple_initiator_socket<SimpleBus>, NR_OF_TARGETS> isocks;
	std::array<PortMapping *, NR_OF_TARGETS> ports;

	SimpleBus(sc_core::sc_module_name) {
		for (auto &s : tsocks) {
			s.register_b_transport(this, &SimpleBus::transport);
			s.register_transport_dbg(this, &SimpleBus::transport_dbg);
		}
	}

	int decode(uint64_t addr) {
		for (unsigned i = 0; i < NR_OF_TARGETS; ++i) {
			if (ports[i]->contains(addr))
				return i;
		}
		return -1;
	}

	void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
		auto addr = trans.get_address();
		auto id = decode(addr);

		if (id < 0) {
			trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
			return;
		}

		trans.set_address(ports[id]->global_to_local(addr));
		isocks[id]->b_transport(trans, delay);
	}

	unsigned transport_dbg(tlm::tlm_generic_payload &trans) {
		auto addr = trans.get_address();
		auto id = decode(addr);

		if (id < 0) {
			trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
			return 0;
		}

		trans.set_address(ports[id]->global_to_local(addr));
		return isocks[id]->transport_dbg(trans);
	}
};

#include "core/common/bus_lock_if.h"

/*
 * Use this adapter to attach peripherals with write access (e.g. DMA) to the bus.
 * This ensures that those peripherals do not violate the RISC-V LR/SC atomic semantic.
 */
struct PeripheralWriteConnector : sc_core::sc_module {
	tlm_utils::simple_target_socket<PeripheralWriteConnector> tsock;
	tlm_utils::simple_initiator_socket<PeripheralWriteConnector> isock;
	std::shared_ptr<bus_lock_if> bus_lock;

	PeripheralWriteConnector(sc_core::sc_module_name) {
		tsock.register_b_transport(this, &PeripheralWriteConnector::transport);
	}

	void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
		bus_lock->wait_until_unlocked();

		isock->b_transport(trans, delay);

		if (trans.get_response_status() == tlm::TLM_ADDRESS_ERROR_RESPONSE)
			throw std::runtime_error("unable to find target port for address " + std::to_string(trans.get_address()));
	}
};

class BusLock : public bus_lock_if {
	bool locked = false;
	unsigned owner = 0;
	sc_core::sc_event lock_event;

   public:
	virtual void lock(unsigned hart_id) override {
		if (locked && (hart_id != owner)) {
			wait_until_unlocked();
		}

		assert(!locked || (hart_id == owner));
		locked = true;
		owner = hart_id;
	}

	virtual void unlock(unsigned hart_id) override {
		if (locked && (owner == hart_id)) {
			locked = false;
			lock_event.notify(sc_core::SC_ZERO_TIME);
		}
	}

	virtual bool is_locked() override {
		return locked;
	}

	virtual bool is_locked(unsigned hart_id) override {
		return locked && (owner == hart_id);
	}

	virtual void wait_until_unlocked() override {
		while (locked) sc_core::wait(lock_event);
	}
};

#endif  // RISCV_ISA_BUS_H
