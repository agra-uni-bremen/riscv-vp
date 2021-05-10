#include "can.h"

#include <linux/can.h>
#include <linux/can/raw.h>
#include <endian.h>

#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

CAN::CAN() {
	state = State::init;
	status = 0;
	stop = false;

	s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (s < 0) {
		perror("Could not open socket!");
		return;
	}

	memset(&ifr, 0, sizeof(struct ifreq));
	strcpy(ifr.ifr_name, "slcan0");
	if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
		close(s);
		perror("Could not ctl to device");
		return;
	}

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		close(s);
		perror("Could not bind to can family");
		return;
	}

	listener = std::thread(&CAN::listen, this);
}

CAN::~CAN() {
	stop = true;
	if (listener.joinable())
		listener.join();
}

const char* CAN::registerName(uint8_t id) {
	switch (id) {
		/*..*/
		case MCP_CANCTRL:
			return "MCP_CANCTRL";
		case MCP_RXF0SIDH:
			return "MCP_RXF0SIDH";
		/*..*/
		case MCP_CNF1:
			return "MCP_CNF1";
		case MCP_CNF2:
			return "MCP_CNF2";
		case MCP_CNF3:
			return "MCP_CNF3";
		/*..*/
		case MCP_CANINTF:
			return "MCP_CANINTF";

		case MCP_TXB1CTRL:
			return "MCP_TXB1CTRL";
		case MCP_TXB2CTRL:
			return "MCP_TXB2CTRL";

		default:
			return "UNKNOWN-RegName";
	}
}

const char* CAN::regValueName(uint8_t id) {
	switch (id) {
		/*..*/
		case MODE_NORMAL:
			return "MODE_NORMAL";
		case MODE_SLEEP:
			return "MODE_SLEEP";

		case MODE_CONFIG:
			return "MODE_CONFIG";

		case MODE_ONESHOT:
			return "MODE_ONESHOT";

		default:
			return "UNKNOWN-RegValue";
	}
}

const char* CAN::spiInstName(uint8_t id) {
	switch (id) {
		/*..*/
		case MCP_WRITE:
			return "MCP_WRITE";
		case MCP_READ:
			return "MCP_READ";
		case MCP_BITMOD:
			return "MCP_BITMOD";
		case MCP_LOAD_TX0:
			return "MCP_LOAD_TX0";
		case MCP_LOAD_TX1:
			return "MCP_LOAD_TX1";
		case MCP_LOAD_TX2:
			return "MCP_LOAD_TX2";
		case MCP_RTS_TX0:
			return "MCP_RTS_TX0";
		case MCP_RTS_TX1:
			return "MCP_RTS_TX1";
		case MCP_RTS_TX2:
			return "MCP_RTS_TX2";
		case MCP_RTS_ALL:
			return "MCP_RTS_ALL";
		case MCP_READ_RX0:
			return "MCP_READ_RX0";
		case MCP_READ_RX1:
			return "MCP_READ_RX1";
		case MCP_READ_STATUS:
			return "MCP_READ_STATUS";
		case MCP_RX_STATUS:
			return "MCP_RX_STATUS";
		case MCP_RESET:
			return "MCP_RESET";

		default:
			return "UNKNOWN-SpiInst";
	}
}

uint8_t CAN::write(uint8_t byte) {
	uint8_t ret = 0;
	uint8_t whichBuf = 0;
	switch (state) {
		case State::init:
			command(byte);
			ret = 0;
			break;
		case State::readRegister:
			ret = readRegister(byte);
			break;
		case State::writeRegister:
			ret = writeRegister(byte);
			break;
		case State::bitmod:
			ret = modifyRegister(byte);
			break;
		case State::loadTX2:
			whichBuf++;
			// fall-through
		case State::loadTX1:
			whichBuf++;
			// fall-through
		case State::loadTX0:
			ret = loadTxBuf(whichBuf, byte);
			break;
		case State::sendALL:
			whichBuf++;
			// fall-through
		case State::sendTX2:
			whichBuf++;
			// fall-through
		case State::sendTX1:
			whichBuf++;
			// fall-through
		case State::sendTX0:
			ret = sendTxBuf(whichBuf, byte);
			break;
		case State::readRX1:
			whichBuf++;
			// fall-through
		case State::readRX0:
			ret = readRxBuf(whichBuf, byte);
			break;
		case State::getStatus:
			ret = status;  // short enough to be handled here
			state = State::init;
			break;
		default:
			std::cerr << "[CAN] in unknown state!" << std::endl;
	}

	return ret;
}

void CAN::command(uint8_t byte) {
	// std::cout << "[CAN] " << spiInstName(byte);
	switch (byte) {
		case MCP_WRITE:
			state = State::writeRegister;
			break;
		case MCP_READ:
			state = State::readRegister;
			break;
		case MCP_BITMOD:
			state = State::bitmod;
			break;
		case MCP_LOAD_TX0:
			state = State::loadTX0;
			break;
		case MCP_LOAD_TX1:
			state = State::loadTX1;
			break;
		case MCP_LOAD_TX2:
			state = State::loadTX2;
			break;
		case MCP_RTS_TX0:
			state = State::sendTX0;
			break;
		case MCP_RTS_TX1:
			state = State::sendTX1;
			break;
		case MCP_RTS_TX2:
			state = State::sendTX2;
			break;
		case MCP_RTS_ALL:
			state = State::sendALL;
			break;
		case MCP_READ_RX0:
			state = State::readRX0;
			break;
		case MCP_READ_RX1:
			state = State::readRX1;
			break;
		case MCP_READ_STATUS:
			state = State::getStatus;
			break;
		case MCP_RX_STATUS:
			break;
		case MCP_RESET:
			// do reset stuff
			state = State::init;
			break;
		default:
			std::cerr << std::hex << unsigned(byte) << " is unknown command " << std::endl;
	}
	// std::cout << std::endl;
}

uint8_t CAN::readRegister(uint8_t byte) {
	static int16_t selectedRegister = -1;
	uint8_t ret = 0;

	if (selectedRegister < 0) {
		selectedRegister = byte;
		// std::cout << "\t[CAN] read select Register " << registerName(selectedRegister) << std::endl;
		ret = 0;
	} else {
		if (byte != 0) {  // end "auto increment"
			selectedRegister = -1;
			command(byte);
			return 0;
		}
		// std::cout << "[CAN] Read on Register " << registerName(selectedRegister) << std::endl;
		if (selectedRegister > MCP_RXB1SIDH) {
			std::cerr << "Read on Register too high! ";
			ret = 0;
		} else {
			ret = registers[selectedRegister];
		}
		selectedRegister++;
	}
	return ret;
}

uint8_t CAN::writeRegister(uint8_t byte) {
	static int16_t selectedRegister = -1;
	if (selectedRegister < 0) {
		selectedRegister = byte;
		// std::cout << "\t[CAN] Write select Register "  << registerName(selectedRegister) << std::endl;
		return 0;
	} else {
		// std::cout << "[CAN] Write on Register " << registerName(selectedRegister) << std::endl;

		if (selectedRegister > MCP_RXB1SIDH) {
			std::cerr << "Write on Register too high! ";
			return 0;
		} else {
			registers[selectedRegister] = byte;
		}
		selectedRegister = -1;
		state = State::init;
	}
	return 0;
}

uint8_t CAN::modifyRegister(uint8_t byte) {
	static struct ModReg {
		enum : uint8_t { _address = 0, _mask, _data } state;
		uint8_t address;
		uint8_t mask;
		uint8_t data;

		ModReg() : state(_address), address(0), mask(0), data(0){};
	} command;

	// std::cout << "\t[CAN] modReg ";
	switch (command.state) {
		case ModReg::_address:
			command.address = byte;
			command.state = ModReg::_mask;
			// std::cout << "select " << registerName(command.address) << std::endl;
			break;
		case ModReg::_mask:
			command.mask = byte;
			command.state = ModReg::_data;

			// std::cout << "mask " << std::hex << unsigned(byte) << std::endl;
			break;
		case ModReg::_data:
			command.data = byte;
			command.state = ModReg::_address;
			state = State::init;
			// std::cout << "setData " << regValueName(command.data) << std::endl;
			registers[command.address] &= ~command.mask;
			registers[command.address] |= command.data;
			break;
	}
	return 0;
}

uint8_t CAN::loadTxBuf(uint8_t no, uint8_t byte) {
	static struct LoadTX {
		enum {
			id,
			length,
			payload,
		} state = id;
		uint8_t payload_ptr = 0;
	} command;

	switch (command.state) {
		case LoadTX::id:
			// std::cout << "\t[CAN] ID: " << std::hex << unsigned(byte) << std::endl;
			txBuf[no].id[command.payload_ptr++] = byte;
			if (command.payload_ptr == 4) {
				command.payload_ptr = 0;
				command.state = LoadTX::length;
			}
			break;
		case LoadTX::length:
			// std::cout << "\t[CAN] Length : " << std::hex << unsigned(byte) << std::endl;
			command.state = LoadTX::payload;
			txBuf[no].length = byte;
			break;
		case LoadTX::payload:
			// std::cout << "\t[CAN] PL : " << std::hex << unsigned(byte) << std::endl;
			txBuf[no].payload[command.payload_ptr++] = byte;
			if (command.payload_ptr == txBuf[no].length) {
				command = LoadTX();
				state = State::init;
			}
			break;
	}

	return 0;
}

uint8_t CAN::sendTxBuf(uint8_t no, uint8_t) {
	if (no == 4) {
		// todo: Special case to send all buffers
		no = 0;
	}
	struct can_frame frame;
	bool extended;  // this is ignored
	memset(&frame, 0, sizeof(struct can_frame));
	mcp2515_buf_to_id(frame.can_id, extended, txBuf[no].id);
	frame.can_dlc = txBuf[no].length;
	memcpy(frame.data, txBuf[no].payload, txBuf[no].length);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"

	::write(s, &frame, sizeof(struct can_frame));
	;

#pragma GCC diagnostic pop

	// Set 'sent' status ok
	registers[MCP_TXB0CTRL] = 0;
	state = State::readRegister;
	return 0;
}

uint8_t CAN::readRxBuf(uint8_t no, uint8_t) {
	static uint8_t payload_ptr = 0;
	uint8_t ret = rxBuf[no].raw[payload_ptr++];
	if (payload_ptr > rxBuf[no].length + 4) {
		payload_ptr = 0;
		state = State::init;
		status &= ~(1 << no);  // MCP_STAT_RX0IF = 1 << 0
	}
	// std::cout << "[CAN] readRxBuf" << unsigned(no) << " " << unsigned(ret) << std::endl;
	return ret;
}

void CAN::mcp2515_id_to_buf(const unsigned long id, uint8_t* idField, const bool extended) {
	uint16_t canid;

	canid = (uint16_t)(id & 0x0FFFF);

	if (extended) {
		idField[MCP_EID0] = (uint8_t)(canid & 0xFF);
		idField[MCP_EID8] = (uint8_t)(canid >> 8);
		canid = (uint16_t)(id >> 16);
		idField[MCP_SIDL] = (uint8_t)(canid & 0x03);
		idField[MCP_SIDL] += (uint8_t)((canid & 0x1C) << 3);
		idField[MCP_SIDL] |= MCP_TXB_EXIDE_M;
		idField[MCP_SIDH] = (uint8_t)(canid >> 5);
	} else {
		idField[MCP_SIDH] = (uint8_t)(canid >> 3);
		idField[MCP_SIDL] = (uint8_t)((canid & 0x07) << 5);
		idField[MCP_EID0] = 0;
		idField[MCP_EID8] = 0;
	}
}

void CAN::mcp2515_buf_to_id(unsigned& id, bool& extended, uint8_t* idField) {
	extended = false;
	id = 0;

	id = (idField[MCP_SIDH] << 3) + (idField[MCP_SIDL] >> 5);
	if ((idField[MCP_SIDL] & MCP_TXB_EXIDE_M) == MCP_TXB_EXIDE_M) {
		// extended id
		id = (id << 2) + (idField[MCP_SIDL] & 0x03);
		id = (id << 8) + idField[MCP_EID8];
		id = (id << 8) + idField[MCP_EID0];
		extended = true;
	}
}

void CAN::enqueueIncomingCanFrame(const struct can_frame& frame) {
	if ((status & 0b11) == 0b11) {
		// all buffers full
		return;
	}

	for (unsigned i = 0; i < 2; i++) {
		if (!(status & (1 << i)))  // Correnspond to MCP_STAT_RXnIF registers
		{
			// empty buffer
			memset(&rxBuf[i], 0, sizeof(MCPFrame));
			mcp2515_id_to_buf(frame.can_id, rxBuf[i].id);  // FIXME: This will break if extended frame or stuff happens
			rxBuf[i].length = frame.can_dlc;
			memcpy(rxBuf[i].payload, frame.data, frame.can_dlc);
			status |= 1 << i;
			return;
		}
	}
}

void CAN::listen() {
	while (!stop) {
		struct can_frame frame;

		int nbytes = read(s, &frame, sizeof(struct can_frame));

		if (nbytes < 0) {
			perror("can raw socket read");
			continue;
		}

		/* paranoid check ... */
		if (nbytes < static_cast<long>(sizeof(struct can_frame))) {
			fprintf(stderr, "read: incomplete CAN frame\n");
			continue;
		}

		enqueueIncomingCanFrame(frame);
	}
}
