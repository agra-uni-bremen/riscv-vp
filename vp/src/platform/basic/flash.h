#pragma once

#include <fcntl.h>

#include <linux/fs.h>

#include <stdint.h>
#include <sys/ioctl.h>
#include <unistd.h>  //truncate
#include <fstream>   //file IO
#include <iostream>

#include <tlm_utils/simple_target_socket.h>
#include <systemc>

#include "bus.h"

using namespace std;
using namespace sc_core;
using namespace tlm_utils;

template <size_t width>
struct Blockbuffer {
	uint8_t buf[width];
	uint64_t offs;
	bool dirty;
	bool active;
	int fd;
	Blockbuffer(int fileDescriptor) : offs(0), dirty(false), active(false), fd(fileDescriptor){};

	void setPos(uint64_t blockOffset) {
		if (blockOffset != offs || !active) {
			if (dirty && active) {  // commit changes
				writeBlock(offs);
			}
			offs = blockOffset;
			readBlock(blockOffset);
			active = true;
		}
	}
	void setData(const uint8_t* source, uint16_t pos, uint16_t length) {
		assert(active);
		memcpy(&buf[pos], source, length);
		dirty = true;
	}
	void getData(uint8_t* target, uint16_t pos, uint16_t length) {
		assert(active);
		memcpy(target, &buf[pos], length);
	}

	void writeBlock(uint64_t blockOffset) {
		if (lseek64(fd, blockOffset * width, SEEK_SET) < 0) {
			cerr << "Could not seek device: " << strerror(errno) << endl;
			return;
		}
		if (write(fd, buf, width) != width) {
			cerr << "Could not write device: " << strerror(errno) << endl;
			return;
		}
		dirty = false;
	}

	void readBlock(uint64_t blockOffset) {
		if (lseek64(fd, blockOffset * width, SEEK_SET) < 0) {
			cerr << "Could not seek device: " << strerror(errno) << endl;
			return;
		}
		if (read(fd, buf, width) != width) {
			cerr << "Could not read device: " << strerror(errno) << endl;
			return;
		}
		dirty = false;
	}
	void clear() {
		if (active && dirty) {
			writeBlock(offs);
		}
		active = false;
	}
};

struct Flashcontroller : public sc_core::sc_module {
	static const unsigned int BLOCKSIZE = 512;
	static const unsigned int FLASH_ADDR_REG = 0;
	static const unsigned int FLASH_SIZE_REG = sizeof(uint64_t);
	static const unsigned int DATA_ADDR = FLASH_SIZE_REG + sizeof(uint64_t);
	static const unsigned int ADDR_SPACE = DATA_ADDR + BLOCKSIZE;

	simple_target_socket<Flashcontroller> tsock;
	uint8_t blockbufRaw[sizeof(Blockbuffer<BLOCKSIZE>)];
	Blockbuffer<BLOCKSIZE>* blockBuf;

	union {
		uint64_t asInt;
		uint8_t asRaw[8];
	} mTargetBlock;

	union {
		uint64_t asInt;
		uint8_t asRaw[8];
	} mDeviceNumBlocks;

	string mFilepath;
	int mFiledescriptor;

	Flashcontroller(sc_module_name, string& filepath) : blockBuf(nullptr), mFilepath(filepath), mFiledescriptor(-1) {
		tsock.register_b_transport(this, &Flashcontroller::transport);

		if (filepath.length() == 0) {  // No file
			return;
		}

		mFiledescriptor = open(mFilepath.c_str(), O_SYNC | O_RDWR);
		if (mFiledescriptor < 0) {
			cerr << "Could not open device " << mFilepath << ": " << strerror(errno) << endl;
			return;
		}

		if (ioctl(mFiledescriptor, BLKGETSIZE64, &mDeviceNumBlocks.asInt) < 0) {
			cerr << "Could not get size of Device " << mFilepath << ": " << strerror(errno) << endl;
			mDeviceNumBlocks.asInt = lseek64(mFiledescriptor, 0, SEEK_END);
			if (mDeviceNumBlocks.asInt <= 0) {
				close(mFiledescriptor);
				mFiledescriptor = -1;
				return;
			}
			cerr << "Could get size of _file_ " << mFilepath << ": " << mDeviceNumBlocks.asInt << endl;
			mDeviceNumBlocks.asInt /= BLOCKSIZE;
		}
		mTargetBlock.asInt = 0;
		blockBuf = new (blockbufRaw) Blockbuffer<BLOCKSIZE>(mFiledescriptor);
	}

	~Flashcontroller() {
		if (blockBuf != nullptr) {
			blockBuf->clear();
		}
		close(mFiledescriptor);
	}

	void transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
		tlm::tlm_command cmd = trans.get_command();
		unsigned addr = trans.get_address();
		auto* ptr = trans.get_data_ptr();
		auto len = trans.get_data_length();

		assert((addr < DATA_ADDR + BLOCKSIZE) && "Access flashcontroller out of bounds");
		assert(mFiledescriptor >= 0);

		if (/*addr >= FLASH_ADDR_REG &&*/ addr < FLASH_SIZE_REG) {  // Address register
			if (cmd == tlm::TLM_WRITE_COMMAND) {
				memcpy(&mTargetBlock.asRaw[addr - FLASH_ADDR_REG], ptr, len);
			} else if (cmd == tlm::TLM_READ_COMMAND) {
				memcpy(ptr, &mTargetBlock.asRaw[addr - FLASH_ADDR_REG], len);
			} else {
				sc_assert(false && "unsupported tlm command");
			}
			delay += sc_core::sc_time(len * 30, sc_core::SC_NS);
		} else if (addr >= FLASH_SIZE_REG && addr < DATA_ADDR) {  // Size register
			if (cmd == tlm::TLM_READ_COMMAND) {
				memcpy(ptr, &mDeviceNumBlocks.asRaw[addr - FLASH_SIZE_REG], len);
			} else {
				sc_assert(false && "unsupported tlm command");
			}
			delay += sc_core::sc_time(len * 30, sc_core::SC_NS);
		} else {  // Data region
			assert(mTargetBlock.asInt < mDeviceNumBlocks.asInt && "Access Flash out of bounds!");

			blockBuf->setPos(mTargetBlock.asInt);  // Loads Block into buffer
			if (cmd == tlm::TLM_WRITE_COMMAND) {
				blockBuf->setData(ptr, addr - DATA_ADDR, len);
			} else if (cmd == tlm::TLM_READ_COMMAND) {
				blockBuf->getData(ptr, addr - DATA_ADDR, len);
			} else {
				sc_assert(false && "unsupported tlm command");
			}
			// TODO: Add delay based on blockBuf cache
			delay += sc_core::sc_time(len, sc_core::SC_US);
		}
	}
};
