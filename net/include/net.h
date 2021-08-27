/* $Id: net.h,v 1.2 2001/05/31 21:32:23 marvin Exp $
 */
#ifndef __INCLUDE_NET_NET_H__
#define __INCLUDE_NET_NET_H__

enum net_protocol_e {
	PROTO_IPV4,
	PROTO_IPV6,
	PROTO_IPX
};

net_protocol_struct_t *net_get_protocol_handler(enum protocol_e protocol);

#endif
