#include <cstdlib>
#include <ctime>

#include "basic_timer.h"
#include "core/common/clint.h"
#include "display.hpp"
#include "dma.h"
#include "elf_loader.h"
#include "ethernet.h"
#include "fe310_plic.h"
#include "flash.h"
#include "debug_memory.h"
#include "iss.h"
#include "mem.h"
#include "memory.h"
#include "mram.h"
#include "sensor.h"
#include "sensor2.h"
#include "syscall.h"
#include "terminal.h"
#include "dummy_rocc.h"
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

	std::string mram_image;
	std::string flash_device;
	std::string network_device;
	std::string test_signature;

	addr_t mem_size = 1024 * 1024 * 32;  // 32 MB ram, to place it before the CLINT and run the base examples (assume
	                                     // memory start at zero) without modifications
	addr_t mem_start_addr = 0x00000000;
	addr_t mem_end_addr = mem_start_addr + mem_size - 1;
	addr_t clint_start_addr = 0x02000000;
	addr_t clint_end_addr = 0x0200ffff;
	addr_t sys_start_addr = 0x02010000;
	addr_t sys_end_addr = 0x020103ff;
	addr_t rocc_start_addr = ROCC_START_ADDR;
	addr_t rocc_end_addr = ROCC_END_ADDR;	
	addr_t term_start_addr = 0x20000000;
	addr_t term_end_addr = term_start_addr + 16;
	addr_t ethernet_start_addr = 0x30000000;
	addr_t ethernet_end_addr = ethernet_start_addr + 1500;
	addr_t plic_start_addr = 0x40000000;
	addr_t plic_end_addr = 0x41000000;
	addr_t sensor_start_addr = 0x50000000;
	addr_t sensor_end_addr = 0x50001000;
	addr_t sensor2_start_addr = 0x50002000;
	addr_t sensor2_end_addr = 0x50004000;
	addr_t mram_start_addr = 0x60000000;
	addr_t mram_size = 0x10000000;
	addr_t mram_end_addr = mram_start_addr + mram_size - 1;
	addr_t dma_start_addr = 0x70000000;
	addr_t dma_end_addr = 0x70001000;
	addr_t flash_start_addr = 0x71000000;
	addr_t flash_end_addr = flash_start_addr + Flashcontroller::ADDR_SPACE;  // Usually 528 Byte
	addr_t display_start_addr = 0x72000000;
	addr_t display_end_addr = display_start_addr + Display::addressRange;

	bool use_E_base_isa = false;

	OptionValue<unsigned long> entry_point;

	BasicOptions(void) {
        	// clang-format off
		add_options()
			("memory-start", po::value<unsigned int>(&mem_start_addr),"set memory start address")
			("memory-size", po::value<unsigned int>(&mem_size), "set memory size")
			("use-E-base-isa", po::bool_switch(&use_E_base_isa), "use the E instead of the I integer base ISA")
			("entry-point", po::value<std::string>(&entry_point.option),"set entry point address (ISS program counter)")
			("mram-image", po::value<std::string>(&mram_image)->default_value(""),"MRAM image file for persistency")
			("mram-image-size", po::value<unsigned int>(&mram_size), "MRAM image size")
			("flash-device", po::value<std::string>(&flash_device)->default_value(""),"blockdevice for flash emulation")
			("network-device", po::value<std::string>(&network_device)->default_value(""),"name of the tap network adapter, e.g. /dev/tap6")
			("signature", po::value<std::string>(&test_signature)->default_value(""),"output filename for the test execution signature");
        	// clang-format on
	}

	void parse(int argc, char **argv) override {
		Options::parse(argc, argv);

		entry_point.finalize(parse_ulong_option);
		mem_end_addr = mem_start_addr + mem_size - 1;
		assert((mem_end_addr < clint_start_addr || mem_start_addr > display_end_addr) &&
		       "RAM too big, would overlap memory");
		mram_end_addr = mram_start_addr + mram_size - 1;
		assert(mram_end_addr < dma_start_addr && "MRAM too big, would overlap memory");
	}
};

int sc_main(int argc, char **argv) {
	BasicOptions opt;
	opt.parse(argc, argv);

	std::srand(std::time(nullptr));  // use current time as seed for random generator

	tlm::tlm_global_quantum::instance().set(sc_core::sc_time(opt.tlm_global_quantum, sc_core::SC_NS));

	ISS core(0, opt.use_E_base_isa);
	SimpleMemory mem("SimpleMemory", opt.mem_size);
	SimpleTerminal term("SimpleTerminal");
	ELFLoader loader(opt.input_program.c_str());
	SimpleBus<4, 13> bus("SimpleBus");
	CombinedMemoryInterface iss_mem_if("MemoryInterface", core);
	SyscallHandler sys("SyscallHandler");
	FE310_PLIC<1, 64, 96, 32> plic("PLIC");
	CLINT<1> clint("CLINT");
	SimpleSensor sensor("SimpleSensor", 2);
	SimpleSensor2 sensor2("SimpleSensor2", 5);
	BasicTimer timer("BasicTimer", 3);
	SimpleMRAM mram("SimpleMRAM", opt.mram_image, opt.mram_size);
	SimpleDMA dma("SimpleDMA", 4);
	Flashcontroller flashController("Flashcontroller", opt.flash_device);
	EthernetDevice ethernet("EthernetDevice", 7, mem.data, opt.network_device);
	Display display("Display");
	DebugMemoryInterface dbg_if("DebugMemoryInterface");
	DummyRocc rocc("DummyRocc", core);

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
	if (opt.entry_point.available)
		entry_point = opt.entry_point.value;

	loader.load_executable_image(mem, mem.size, opt.mem_start_addr);
	core.init(instr_mem_if, data_mem_if, &clint, entry_point, rv32_align_address(opt.mem_end_addr));
	sys.init(mem.data, opt.mem_start_addr, loader.get_heap_addr());
	sys.register_core(&core);

	if (opt.intercept_syscalls)
		core.sys = &sys;

	// address mapping
	bus.ports[0] = new PortMapping(opt.mem_start_addr, opt.mem_end_addr);
	bus.ports[1] = new PortMapping(opt.clint_start_addr, opt.clint_end_addr);
	bus.ports[2] = new PortMapping(opt.plic_start_addr, opt.plic_end_addr);
	bus.ports[3] = new PortMapping(opt.term_start_addr, opt.term_end_addr);
	bus.ports[4] = new PortMapping(opt.sensor_start_addr, opt.sensor_end_addr);
	bus.ports[5] = new PortMapping(opt.dma_start_addr, opt.dma_end_addr);
	bus.ports[6] = new PortMapping(opt.sensor2_start_addr, opt.sensor2_end_addr);
	bus.ports[7] = new PortMapping(opt.mram_start_addr, opt.mram_end_addr);
	bus.ports[8] = new PortMapping(opt.flash_start_addr, opt.flash_end_addr);
	bus.ports[9] = new PortMapping(opt.ethernet_start_addr, opt.ethernet_end_addr);
	bus.ports[10] = new PortMapping(opt.display_start_addr, opt.display_end_addr);
	bus.ports[11] = new PortMapping(opt.sys_start_addr, opt.sys_end_addr);
	bus.ports[12] = new PortMapping(opt.rocc_start_addr, opt.rocc_end_addr);

	// connect TLM sockets
	iss_mem_if.isock.bind(bus.tsocks[0]);
	dbg_if.isock.bind(bus.tsocks[3]);

	core.isock.bind(rocc.tsocks[0]);
	rocc.isocks[0].bind(core.tsock);

	PeripheralWriteConnector rocc_connector("DummyRocc-Connector");  // to respect ISS bus locking
	rocc_connector.isock.bind(bus.tsocks[2]);
	rocc.isocks[1].bind(rocc_connector.tsock);
	rocc_connector.bus_lock = bus_lock;

	PeripheralWriteConnector dma_connector("SimpleDMA-Connector");  // to respect ISS bus locking
	dma_connector.isock.bind(bus.tsocks[1]);
	dma.isock.bind(dma_connector.tsock);
	dma_connector.bus_lock = bus_lock;

	bus.isocks[0].bind(mem.tsock);
	bus.isocks[1].bind(clint.tsock);
	bus.isocks[2].bind(plic.tsock);
	bus.isocks[3].bind(term.tsock);
	bus.isocks[4].bind(sensor.tsock);
	bus.isocks[5].bind(dma.tsock);
	bus.isocks[6].bind(sensor2.tsock);
	bus.isocks[7].bind(mram.tsock);
	bus.isocks[8].bind(flashController.tsock);
	bus.isocks[9].bind(ethernet.tsock);
	bus.isocks[10].bind(display.tsock);
	bus.isocks[11].bind(sys.tsock);
	bus.isocks[12].bind(rocc.tsocks[1]);

	// connect interrupt signals/communication
	plic.target_harts[0] = &core;
	clint.target_harts[0] = &core;
	sensor.plic = &plic;
	dma.plic = &plic;
	timer.plic = &plic;
	sensor2.plic = &plic;
	ethernet.plic = &plic;

	std::vector<debug_target_if *> threads;
	threads.push_back(&core);

	core.trace = opt.trace_mode;  // switch for printing instructions
	if (opt.use_debug_runner) {
		auto server = new GDBServer("GDBServer", threads, &dbg_if, opt.debug_port);
		new GDBServerRunner("GDBRunner", server, &core);
	} else {
		new DirectCoreRunner(core);
	}

	sc_core::sc_start();

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
