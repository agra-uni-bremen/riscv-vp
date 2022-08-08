#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#include <tlm_utils/simple_target_socket.h>
#include <systemc>

#include "core/common/irq_if.h"
#include "util/memory_map.h"
#include "util/tlm_map.h"
#include "fu540_plic.h"

inline uint32_t GET_IDX(uint32_t& irq) { return irq / 32; }
inline uint32_t GET_OFF(uint32_t& irq) { return 1 << irq % 32; }

/**
 * TODO: Ensure that irq 0 is hardwired to zero
 * TODO: FE310 raises external interrupt during interrupt completion
 */

static void assert_addr(size_t start, size_t end, RegisterRange *range) {
	assert(range->start == start && range->end + 1 == end + sizeof(uint32_t));
}

FU540_PLIC::FU540_PLIC(sc_core::sc_module_name, unsigned harts) {
	target_harts = std::vector<external_interrupt_target *>(harts, NULL);

	/* Values copied from FE310_PLIC */
	clock_cycle = sc_core::sc_time(10, sc_core::SC_NS);

	create_registers();
	tsock.register_b_transport(this, &FU540_PLIC::transport);

	SC_THREAD(run);
};

void FU540_PLIC::create_registers(void) {
	regs_interrupt_priorities.post_write_callback =
		std::bind(&FU540_PLIC::write_irq_prios, this, std::placeholders::_1);

	/* make pending interrupts read-only */
	regs_pending_interrupts.pre_write_callback =
		[] (RegisterRange::WriteInfo) { return false; };

	/* The priorities end address, as documented in the FU540-C000
	 * manual, is incorrect <https://github.com/riscv/opensbi/pull/138> */
	assert_addr(0x4, 0xD4, &regs_interrupt_priorities);
	assert_addr(0x1000, 0x1004, &regs_pending_interrupts);

	register_ranges.push_back(&regs_interrupt_priorities);
	register_ranges.push_back(&regs_pending_interrupts);

	/* create IRQ enable and context registers */
	create_hart_regs(ENABLE_BASE, ENABLE_PER_HART, enabled_irqs);
	create_hart_regs(CONTEXT_BASE, CONTEXT_PER_HART, hart_context);

	/* only supports "naturally aligned 32-bit memory accesses" */
	for (size_t i = 0; i < register_ranges.size(); i++)
		register_ranges[i]->alignment = sizeof(uint32_t);
}

void FU540_PLIC::create_hart_regs(uint64_t addr, uint64_t inc, hartmap &map) {
	auto add_reg = [this, addr] (unsigned int h, PrivilegeLevel l, uint64_t a) {
		RegisterRange *r = new RegisterRange(a, HART_REG_SIZE);
		if (addr == CONTEXT_BASE) {
			r->pre_read_callback = std::bind(&FU540_PLIC::read_hartctx, this, std::placeholders::_1, h, l);
			r->post_write_callback = std::bind(&FU540_PLIC::write_hartctx, this, std::placeholders::_1, h, l);
		}

		register_ranges.push_back(r);
		return r;
	};

	for (size_t i = 0; i < target_harts.size(); i++) {
		RegisterRange *mreg, *sreg;

		mreg = add_reg(i, MachineMode, addr);
		sreg = mreg; /* for hart0 */

		if (i != 0) { /* hart 0 only supports m-mode interrupts */
			addr += inc;
			sreg = add_reg(i, SupervisorMode, addr);
		}

		map[i] = new HartConfig(*mreg, *sreg);
		addr += inc;
	}
}

void FU540_PLIC::transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
	delay += 4 * clock_cycle; /* copied from FE310_PLIC */
	vp::mm::route("FU540_PLIC", register_ranges, trans, delay);
};

void FU540_PLIC::gateway_trigger_interrupt(uint32_t irq) {
	if (irq == 0 || irq > NUMIRQ)
		throw std::invalid_argument("IRQ value is invalid");

	pending_interrupts[GET_IDX(irq)] |= GET_OFF(irq);
	e_run.notify(clock_cycle);
};

bool FU540_PLIC::read_hartctx(RegisterRange::ReadInfo t, unsigned int hart, PrivilegeLevel level) {
	assert(t.addr % sizeof(uint32_t) == 0);
	assert(t.size == sizeof(uint32_t));

	if (is_claim_access(t.addr)) {
		unsigned int irq = next_pending_irq(hart, level, true);

		/* if there is no pending irq zero needs to be written
		 * to the claim register. next_pending_irq returns 0 in
		 * this case so no special handling required. */

		switch (level) {
		case MachineMode:
			hart_context[hart]->m_mode[1] = irq;
			break;
		case SupervisorMode:
			hart_context[hart]->s_mode[1] = irq;
			break;
		default:
			assert(0);
			break;
		}

		/* successful claim also clears the pending bit */
		if (irq != 0)
			clear_pending(irq);
	}

	return true;
}

void FU540_PLIC::write_hartctx(RegisterRange::WriteInfo t, unsigned int hart, PrivilegeLevel level) {
	assert(t.addr % sizeof(uint32_t) == 0);
	assert(t.size == sizeof(uint32_t));

	if (is_claim_access(t.addr)) {
		target_harts[hart]->clear_external_interrupt(level);
	} else { /* access to priority threshold */
		uint32_t *thr;

		switch (level) {
		case MachineMode:
			thr = &hart_context[hart]->m_mode[0];
			break;
		case SupervisorMode:
			thr = &hart_context[hart]->s_mode[0];
			break;
		default:
			assert(0);
			break;
		}

		*thr = std::min(*thr, uint32_t(MAX_THR));
	}
}

void FU540_PLIC::write_irq_prios(RegisterRange::WriteInfo t) {
	size_t idx = t.addr / sizeof(uint32_t);
	assert(idx <= NUMIRQ);

	auto &elem = interrupt_priorities[idx];
	elem = std::min(elem, uint32_t(MAX_PRIO));
}

void FU540_PLIC::run(void) {
	for (;;) {
		sc_core::wait(e_run);

		for (size_t i = 0; i < target_harts.size(); i++) {
			PrivilegeLevel lvl;
			if (has_pending_irq(i, &lvl)) {
				target_harts[i]->trigger_external_interrupt(lvl);
			}
		}
	}
}

/* Returns next enabled pending interrupt with highest priority */
unsigned int FU540_PLIC::next_pending_irq(unsigned int hart, PrivilegeLevel lvl, bool ignth) {
	assert(!(hart == 0 && lvl == SupervisorMode));

	HartConfig *conf = enabled_irqs[hart];
	unsigned int selirq = 0, maxpri = 0;

	for (unsigned irq = 1; irq <= NUMIRQ; irq++) {
		if (!conf->is_enabled(irq, lvl) || !is_pending(irq))
			continue;

		uint32_t prio = interrupt_priorities[irq - 1];
		if (!ignth && prio < get_threshold(hart, lvl))
			continue;

		if (prio > maxpri) {
			maxpri = prio;
			selirq = irq;
		}
	}

	return selirq;
}

bool FU540_PLIC::has_pending_irq(unsigned int hart, PrivilegeLevel *level) {
	if (hart != 0 && next_pending_irq(hart, SupervisorMode, false) > 0) {
		*level = SupervisorMode;
		return true;
	} else if (next_pending_irq(hart, MachineMode, false) > 0) {
		*level = MachineMode;
		return true;
	} else {
		return false;
	}
}

uint32_t FU540_PLIC::get_threshold(unsigned int hart, PrivilegeLevel level) {
	if (hart == 0 && level == SupervisorMode)
		throw std::invalid_argument("hart0 doesn't support SupervisorMode");

	HartConfig *conf = hart_context[hart];
	switch (level) {
	case MachineMode:
		return conf->m_mode[0];
		break;
	case SupervisorMode:
		return conf->s_mode[0];
		break;
	default:
		throw std::invalid_argument("Invalid PrivilegeLevel");
	}
}

void FU540_PLIC::clear_pending(unsigned int irq) {
	assert(irq > 0 && irq <= NUMIRQ);
	pending_interrupts[GET_IDX(irq)] &= ~(GET_OFF(irq));
}

bool FU540_PLIC::is_pending(unsigned int irq) {
	assert(irq > 0 && irq <= NUMIRQ);
	return pending_interrupts[GET_IDX(irq)] & GET_OFF(irq);
}

bool FU540_PLIC::is_claim_access(uint64_t addr) {
	unsigned idx = addr / sizeof(uint32_t);
	return (idx % 2) == 1;
}

bool FU540_PLIC::HartConfig::is_enabled(unsigned int irq, PrivilegeLevel level) {
	assert(irq > 0 && irq <= NUMIRQ);

	unsigned int idx = GET_IDX(irq);
	unsigned int off = GET_OFF(irq);

	switch (level) {
	case MachineMode:
		return m_mode[idx] & off;
	case SupervisorMode:
		return s_mode[idx] & off;
	default:
		assert(0);
	}

	return false;
}
