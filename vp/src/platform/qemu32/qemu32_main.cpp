#include <cstdlib>
#include <ctime>

//#include "core/common/clint.h"
#include "qemu32_clint.h"
#include "elf_loader.h"
#include "debug_memory.h"
#include "qemu32_uart.h"
// #include "uart16550.h"
#include "iss.h"
#include "mem.h"
#include "memory.h"
#include "memory_mapped_file.h"
#include "syscall.h"
#include "util/options.h"
#include "platform/common/options.h"
#include "gdb-mc/gdb_server.h"
#include "gdb-mc/gdb_runner.h"

#include <boost/io/ios_state.hpp>
#include <boost/program_options.hpp>
#include <iomanip>
#include <iostream>


using namespace rv32;
namespace po = boost::program_options;

class BasicOptions : public Options {
   public:
	typedef unsigned int addr_t;

	std::string network_device;
	std::string test_signature;

	addr_t clint_start_addr = 0x02000000;
	addr_t clint_end_addr = 0x0200ffff;

	addr_t sys_start_addr = 0x02010000;
	addr_t sys_end_addr = 0x020103ff; // handled by using the ecall in qemu

	addr_t uart_start_addr = 0x10000000;
	addr_t uart_end_addr = uart_start_addr + 0xff;

	addr_t ram_size = 32; // in unit MB
	addr_t mem_size = ram_size * 1024 * 1024;
	addr_t mem_start_addr = 0x80000000;
	addr_t mem_end_addr = mem_start_addr + mem_size - 1;

	bool quiet = false;
	bool use_E_base_isa = false;

	OptionValue<unsigned long> entry_point;

	BasicOptions(void) {
		// clang-format off
		add_options()
			("ram-size", po::value<unsigned int>(&ram_size), "define the memory size (MB)")
			("use-E-base-isa", po::bool_switch(&use_E_base_isa), "use the E instead of the I integer base ISA")
			("entry-point", po::value<std::string>(&entry_point.option),"set entry point address (ISS program counter)");
		// clang-format on
	};

	void printValues(std::ostream& os) const override {
		os << std::hex;
		os << "mem_start_addr:\t" << +mem_start_addr << std::endl;
		os << "mem_end_addr:\t"   << +mem_end_addr   << std::endl;
		static_cast <const Options&>( *this ).printValues(os);
	}

	void parse(int argc, char **argv) override {
		Options::parse(argc, argv);

		std::cout << "RAM Size: " << ram_size << " MB" << std::endl;
		std::cout << "Entry Point: " << entry_point.option << std::endl;

		entry_point.finalize(parse_ulong_option);
		mem_size = ram_size * 1024 * 1024;
		mem_end_addr = mem_start_addr + mem_size - 1;
		std::cout << "Memory Start Addr: 0x" << std::hex << mem_start_addr << std::endl;
		std::cout << "Memory End Addr: 0x" << std::hex << mem_end_addr << std::endl;

	}
};


int sc_main(int argc, char **argv) {
	BasicOptions opt;
	opt.parse(argc, argv);

	std::srand(std::time(nullptr));  // use current time as seed for random generator

	tlm::tlm_global_quantum::instance().set(sc_core::sc_time(opt.tlm_global_quantum, sc_core::SC_NS));
	ISS core(0, opt.use_E_base_isa); // warn 2
	SimpleMemory mem("SimpleMemory", opt.mem_size);
	ELFLoader loader(opt.input_program.c_str());
	SimpleBus<2, 4> bus("SimpleBus");
	CombinedMemoryInterface iss_mem_if("MemoryInterface", core);  // warn 1, loop
	SyscallHandler sys("SyscallHandler");
	Qemu32CLINT<1> qemu32_clint("Qemu32CLINT");
	// UART16550 qemu32_uart("Qemu32UART");
	Qemu32UART qemu32_uart("Qemu32UART", 6);
	DebugMemoryInterface dbg_if("DebugMemoryInterface");

	MemoryDMI dmi = MemoryDMI::create_start_size_mapping(mem.data, opt.mem_start_addr, mem.size);
	InstrMemoryProxy instr_mem(dmi, core);

	std::shared_ptr<BusLock> bus_lock = std::make_shared<BusLock>();
	iss_mem_if.bus_lock = bus_lock;

	instr_memory_if *instr_mem_if = &iss_mem_if;
	data_memory_if *data_mem_if = &iss_mem_if;
	if (opt.use_instr_dmi)
		instr_mem_if = &instr_mem;
	if (opt.use_data_dmi) {
		iss_mem_if.dmi_ranges.emplace_back(dmi);
	}

	uint64_t entry_point = loader.get_entrypoint();
	std::cout << "Entry point from ELF: 0x" << std::hex << loader.get_entrypoint() << std::endl;
	if (opt.entry_point.available) {
		std::cout << "Using entry point from options: 0x" << std::hex << opt.entry_point.value << std::endl;
		entry_point = opt.entry_point.value;
	} else {
		std::cout << "Entry point not available" << std::endl;
	}
	loader.load_executable_image(mem, mem.size, opt.mem_start_addr);

	/*
	 * The rv32 calling convention defaults to 32 bit, but as this Config is
	 * mainly used together with the syscall handler, this helps for certain floats.
	 * https://github.com/riscv-non-isa/riscv-elf-psabi-doc/blob/master/riscv-elf.adoc
	 */
	core.init(instr_mem_if, data_mem_if, &qemu32_clint, entry_point, rv32_align_address(opt.mem_end_addr));
	sys.init(mem.data, opt.mem_start_addr, loader.get_heap_addr());
	sys.register_core(&core);

	if (opt.intercept_syscalls)
		core.sys = &sys;
	core.error_on_zero_traphandler = opt.error_on_zero_traphandler;

	// address mapping
	{
		unsigned it = 0;
		bus.ports[it++] = new PortMapping(opt.mem_start_addr, opt.mem_end_addr);
		bus.ports[it++] = new PortMapping(opt.clint_start_addr, opt.clint_end_addr);
		bus.ports[it++] = new PortMapping(opt.uart_start_addr, opt.uart_end_addr);
		bus.ports[it++] = new PortMapping(opt.sys_start_addr, opt.sys_end_addr);
	}

	// connect TLM sockets
	iss_mem_if.isock.bind(bus.tsocks[0]);
	dbg_if.isock.bind(bus.tsocks[1]);

	{
		unsigned it = 0;
		bus.isocks[it++].bind(mem.tsock);
		bus.isocks[it++].bind(qemu32_clint.tsock);
		bus.isocks[it++].bind(qemu32_uart.tsock);
		bus.isocks[it++].bind(sys.tsock);
	}

	// connect interrupt signals/communication
	qemu32_clint.target_harts[0] = &core;

	std::vector<debug_target_if *> threads;
	threads.push_back(&core);

	core.trace = opt.trace_mode;  // switch for printing instructions
	if (opt.use_debug_runner) {
		auto server = new GDBServer("GDBServer", threads, &dbg_if, opt.debug_port);
		new GDBServerRunner("GDBRunner", server, &core);
	} else {
		new DirectCoreRunner(core);
	}
	if (opt.quiet)
		sc_core::sc_report_handler::set_verbosity_level(sc_core::SC_NONE);
	sc_core::sc_start();
	if (!opt.quiet)
		core.show();
	if (opt.test_signature != "") {
		auto begin_sig = loader.get_begin_signature_address();
		auto end_sig = loader.get_end_signature_address();

		{
			boost::io::ios_flags_saver ifs(cout);
			std::cout << std::hex;
			std::cout << "begin_signature: " << begin_sig << std::endl;
			std::cout << "end_signature: " << end_sig << std::endl;
			std::cout << "signature output file: " << opt.test_signature << std::endl;
		}

		assert(end_sig >= begin_sig);
		assert(begin_sig >= opt.mem_start_addr);
		std::cout << "sig2\n" << std::endl;
		auto begin = begin_sig - opt.mem_start_addr;
		auto end = end_sig - opt.mem_start_addr;

		ofstream sigfile(opt.test_signature, ios::out);

		auto n = begin;
		while (n < end) {
			sigfile << std::hex << std::setw(2) << std::setfill('0') << (unsigned)mem.data[n];
			++n;
		}
	}

	return 0;
}