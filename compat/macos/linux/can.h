#ifndef BYTESWAP_H
#define BYTESWAP_H

#define PF_CAN 29
#define AF_CAN PF_CAN

#define CAN_MAX_DLEN	8
#define CANFD_MAX_DLEN 64

/* protocols of protocol family PF_CAN */
#define CAN_RAW			1 /* RAW sockets */
#define CAN_BCM			2 /* Broadcast Manager */
#define CAN_TP16		3 /* VAG Transport Protocol v1.6 */
#define CAN_TP20		4 /* CAG Transport Protocol v2.0 */
#define CAN_MCNET		5 /* Bosch MCNet */
#define CAN_ISOTP		6 /* ISO 15765-2 Transport Protocol */

typedef uint32_t canid_t;

struct can_frame {
	canid_t can_id;		/* 32 bit CAN_ID + EFF/RTR/ERR flags */
	uint8_t	can_dlc;	/* frame payload length in byte (0 .. CAN_MAX_DLEN) */
	uint8_t	__pad;		/* padding */
	uint8_t	__res0;		/* reserved / padding */
	uint8_t	__res1;		/* reserved / padding */
	uint8_t	data[CAN_MAX_DLEN] __attribute__((aligned(8)));
};


struct sockaddr_can {
	uint8_t can_family;
	int         can_ifindex;
	union {
		/* transport protocol class address information (e.g. ISOTP) */
		struct { canid_t rx_id, tx_id; } tp;
		
		/* reserved for future CAN protocols address information */
	} can_addr;
};

#endif

