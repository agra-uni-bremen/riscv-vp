#ifndef RISCV_ISA_MEMORY_H
#define RISCV_ISA_MEMORY_H

#include <stdint.h>
#include <boost/iostreams/device/mapped_file.hpp>
#include <iostream>

#include "bus.h"
#include "load_if.h"

#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/payload_extension.h>
#include <systemc>

/*spandan--------------------*/
#define BYTES_PER_TAG 8
// #define TAG_BITS_PER_ROOT 128
/*spandan--------------------*/

struct SimpleMemory : public sc_core::sc_module, public load_if {
	tlm_utils::simple_target_socket<SimpleMemory> tsock;

	uint8_t *data;
	uint32_t size;
	uint32_t offset; //spandan : tag offset
	bool read_only;

	SimpleMemory(sc_core::sc_module_name, uint32_t size, bool read_only = false)
	    : data(new uint8_t[size]()), size(size), read_only(read_only) {
		offset = size/(8*BYTES_PER_TAG); //spandan : No TAGROOT
		tsock.register_b_transport(this, &SimpleMemory::transport);
		tsock.register_get_direct_mem_ptr(this, &SimpleMemory::get_direct_mem_ptr);
		tsock.register_transport_dbg(this, &SimpleMemory::transport_dbg);
	}

	~SimpleMemory(void) {
		delete[] data;
	}

	void load_data(const char *src, uint64_t dst_addr, size_t n) override {
		assert(dst_addr + n <= size);
		memcpy(&data[dst_addr], src, n);
	}

	void load_zero(uint64_t dst_addr, size_t n) override {
		assert(dst_addr + n <= size);
		memset(&data[dst_addr], 0, n);
	}

	void load_binary_file(const std::string &filename, unsigned addr) {
		boost::iostreams::mapped_file_source f(filename);
		assert(f.is_open());
		write_data(addr, (const uint8_t *)f.data(), f.size());
	}

	void write_data(unsigned addr, const uint8_t *src, unsigned num_bytes) {
		assert(addr + num_bytes <= size);

		memcpy(data + addr, src, num_bytes);
	}

	void read_data(unsigned addr, uint8_t *dst, unsigned num_bytes) {
		assert(addr + num_bytes <= size);

		memcpy(dst, data + addr, num_bytes);
	}

	void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
		transport_dbg(trans);
		delay += sc_core::sc_time(10, sc_core::SC_NS);
	}

	unsigned transport_dbg(tlm::tlm_generic_payload &trans) {
		tlm::tlm_command cmd = trans.get_command();
		unsigned addr = trans.get_address();
		auto *ptr = trans.get_data_ptr();
		auto len = trans.get_data_length();
		tlm_utils::Type datatype = tlm_utils::Type::DATA; //spandan : define a variable for datatype
		//uint32_t root_offset; //spandan : calculate offset for TAGROOT if necessary

		assert(addr < (size-offset)); //spandan : account for tag offset

		/*spandan-----------------------------------*/
		auto* ext = trans.get_extension<tlm_utils::PayloadExtension>();
		if (ext) {
			datatype = ext->payload_type;
		}
		/*spandan-----------------------------------*/

		if (cmd == tlm::TLM_WRITE_COMMAND) {
		/*spandan-----------------------------------*/
			switch (datatype) {
				case tlm_utils::Type::RESET:
					load_zero(0, size); //for TESTRIG
					break;
				case tlm_utils::Type::TAG:
					addr = (size - offset) + addr / ( 8 * (BYTES_PER_TAG) );
					write_data(addr, ptr, len);
					break;
				case tlm_utils::Type::CAPABILITY:
					//Not sure if this is needed---------------------------
					if ( is_cap_not_aligned(addr) )
						throw std::runtime_error("Alignment error at address " + std::to_string(addr));
					//Not sure if this is needed---------------------------
					write_data(addr, ptr, len);
					break;
				case tlm_utils::Type::DATA:
					write_data(addr, ptr, len);
					break;
				default:
					write_data(addr, ptr, len);
					break;
			}
		/*spandan-----------------------------------*/
		} else if (cmd == tlm::TLM_READ_COMMAND) {
		/*spandan-----------------------------------*/
			switch (datatype) {
				case tlm_utils::Type::TAG:
					addr = (size - offset) + addr / ( 8 * (BYTES_PER_TAG) );
					read_data(addr, ptr, len);
					break;
				case tlm_utils::Type::CAPABILITY:
					//Not sure if this is needed---------------------------
					if ( is_cap_not_aligned(addr) )
						throw std::runtime_error("Alignment error at address " + std::to_string(addr));
					//Not sure if this is needed---------------------------
					read_data(addr, ptr, len);
					break;
				case tlm_utils::Type::DATA:
					read_data(addr, ptr, len);
					break;
				default:
					read_data(addr, ptr, len);
					break;
			}
		/*spandan-----------------------------------*/
		} else {
			sc_assert(false && "unsupported tlm command");
		}

		return len;
	}

	bool get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi) {
		(void)trans;
		dmi.set_start_address(0);
		dmi.set_end_address(size);
		dmi.set_dmi_ptr(data);
		if (read_only)
			dmi.allow_read();
		else
			dmi.allow_read_write();
		return true;
	}

/*spandan-----------------------------*/
	bool is_cap_not_aligned(uint64_t addr) {
		return (addr % BYTES_PER_TAG);
	}
/*spandan-----------------------------*/
};

#endif  // RISCV_ISA_MEMORY_H
