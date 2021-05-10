#ifndef CAN_RAW_H
#define CAN_RAW_H

#define SIOCGIFINDEX _IOWR('i', 140, struct ifreq) 
#define ifr_ifindex ifr_ifru.ifru_intval    /* interface index      */

#endif