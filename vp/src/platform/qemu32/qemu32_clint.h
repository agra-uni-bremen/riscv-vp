#pragma once

#include "clint_if.h"
#include "irq_if.h"

#include <cstdlib>
#include <cstring>

#include <systemc>
#include <tlm_utils/simple_target_socket.h>
#include "util/memory_map.h"

// mtime must increment at constant frequency, and the platform must provide a
// mechanism for determining the period of an mtime tick.

// A machine timer interrupt becomes pending whenever mtime contains a value greater than or equal to mtimecmp, treating the
// values as unsigned integers.
// The interrupt remains posted until mtimecmp becomes greater than mtime (typically as a result of writing mtimecmp).
// The interrupt will only be taken if interrupts are enabled and the MTIE bit is set in the mie register.
template <unsigned NumberOfCores>
struct Qemu32CLINT : public clint_if, public sc_core::sc_module {

	static_assert(NumberOfCores < 4096, "out of bound");  // stay within the allocated address range

	static constexpr uint64_t scaler = 1000000;  // scale from PS resolution (default in SystemC) to US
	                                                      // resolution (apparently required by FreeRTOS)

	tlm_utils::simple_target_socket<Qemu32CLINT> tsock;

	sc_core::sc_time clock_cycle = sc_core::sc_time(100, sc_core::SC_NS);
	sc_core::sc_event irq_event;

	// std::cout << "Number of cores: " << NumberOfCores << std::endl;

	RegisterRange regs_mtime{0xBFF8, 8};
	IntegerView<uint64_t> mtime{regs_mtime};

	RegisterRange regs_mtimecmp{0x4000, 8 * NumberOfCores};
	ArrayView<uint64_t> mtimecmp{regs_mtimecmp};


	RegisterRange regs_msip{0x0, 4 * NumberOfCores};
	ArrayView<uint32_t> msip{regs_msip};

	std::vector<RegisterRange *> register_ranges{&regs_mtime, &regs_mtimecmp, &regs_msip};

	std::array<clint_interrupt_target *, NumberOfCores> target_harts{};

	SC_HAS_PROCESS(Qemu32CLINT);

	Qemu32CLINT(sc_core::sc_module_name) {
		tsock.register_b_transport(this, &Qemu32CLINT::transport);

		regs_mtimecmp.alignment = 4;
		regs_msip.alignment = 4;
		regs_mtime.alignment = 4;

		regs_mtime.pre_read_callback = std::bind(&Qemu32CLINT::pre_read_mtime, this, std::placeholders::_1);
		regs_mtimecmp.post_write_callback = std::bind(&Qemu32CLINT::post_write_mtimecmp, this, std::placeholders::_1);
		regs_msip.post_write_callback = std::bind(&Qemu32CLINT::post_write_msip, this, std::placeholders::_1);

		SC_THREAD(run);
	}

	uint64_t update_and_get_mtime() override {
		auto now = sc_core::sc_time_stamp().value() / scaler;
		if (now > mtime)
			mtime = now;  // do not update backward in time (e.g. due to local quantums in tlm transaction processing)
		return mtime;
	}

	void run() {
		while (true) {
			sc_core::wait(irq_event);
			// std::cout << "---------[vp::clint] process ---------" << std::endl;

			update_and_get_mtime();

			for (unsigned i = 0; i < NumberOfCores; ++i) {
				auto cmp = mtimecmp[i];
				// std::cout << "[vp::clint] process mtimecmp[" << i << "]=" << cmp << ", mtime=" << mtime << std::endl;
				if (cmp > 0 && mtime >= cmp) {
					// std::cout << "[vp::clint] set timer interrupt for core " << i << std::endl;
					target_harts[i]->trigger_timer_interrupt(true);
				} else {
					// std::cout << "[vp::clint] unset timer interrupt for core " << i << std::endl;
					target_harts[i]->trigger_timer_interrupt(false);
					if (cmp > 0 && cmp < UINT64_MAX) {
						auto time = sc_core::sc_time::from_value(mtime * scaler);
						auto goal = sc_core::sc_time::from_value(cmp * scaler);
						// std::cout << "[vp::clint] time=" << time << std::endl;
						// std::cout << "[vp::clint] goal=" << goal << std::endl;
						// std::cout << "[vp::clint] goal-time=delay=" << goal-time << std::endl;
						irq_event.notify(goal - time);
					}
				}
			}
		}
	}

	bool pre_read_mtime(RegisterRange::ReadInfo t) {
		sc_core::sc_time now = sc_core::sc_time_stamp() + t.delay;

		mtime.write(now.value() / scaler);

		return true;
	}

	void post_write_mtimecmp(RegisterRange::WriteInfo t) {
		// std::cout << "[vp::clint] write mtimecmp[addr=" << t.addr << "]=" << mtimecmp[t.addr / 8] << ", mtime=" <<
		// mtime << std::endl;
		irq_event.notify(t.delay);
	}

	void post_write_msip(RegisterRange::WriteInfo t) {
		assert(t.addr % 4 == 0);
		unsigned idx = t.addr / 4;
		msip[idx] &= 0x1;
		target_harts[idx]->trigger_software_interrupt(msip[idx] != 0);
	}

	void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
		delay += 2 * clock_cycle;

		vp::mm::route("CLINT", register_ranges, trans, delay);
	}

};
