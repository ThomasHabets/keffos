/* $Id: if.h,v 1.2 2001/05/31 21:32:23 marvin Exp $
 */
#ifndef __INCLUDE_NET_NIC_H__
#define __INCLUDE_NET_NIC_H__

enum nic_type_e {
	NET_IF_ETHER,
	NET_IF_MODEM,
	NET_IF_CABLEMODEM,
	NET_IF_ISDN,
	NET_IF_AX25
};

typedef struct net_if_s {
	int type;
	char *desc;
	
	/*
	 * kbits/sec, this gives us 512 GByte/s to define,
	 * should be enough for our target
	 */
	__uint32 speed;
	struct net_hwa_t hwa;
} net_if_t;

#endif
