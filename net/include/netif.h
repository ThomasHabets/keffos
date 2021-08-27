/* $Id: netif.h,v 1.2 2001/05/31 21:32:23 marvin Exp $
 */
#ifndef __INCLUDE_NET_NETIF_H__
#define __INCLUDE_NET_NETIF_H__

#include <keffos/nic.h>

typedef enum protocol_e {
	PROTO_IPV4,
	PROTO_IPV6,
	PROTO_IPX
} protocol_t;

typedef struct netif_s {
	char *fqdn;                    // FQDN
	char *ifname;                  // ifname (eth0, eth0:1, etc.)
	
	char state;                    // up/down
	protocol_t proto;              // ipv4/ipx/ipv6
	void *proto_info;              // addresses & stuff
	int mtu;                       // MTU
	nic_t nic;                     // nic info
	u64 speedlimit;                // bps soft limit
	u64 lastchange;                // time of last update
	int oqlen;                     // queue length
	
	// Statistics:
	u64 ioctets;                   // raw data recvd
	u64 ooctets;                   // raw data sent
	
	u64 iucast;                    // in unicast
	u64 inucast;                   // in non-unicast
	u64 oucast;                    // out unicast
	u64 onucast;                   // out non-unicast
	
	u64 idiscard;                  // dropped in packets
	u64 odiscard;                  // dropped out packets
	
	u64 ierrors;                   // in errors
	u64 oerrors;                   // out errors
	
	u64 iunkprot;                  // unknown protocol
} netif_t;
#endif
