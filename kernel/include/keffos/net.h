/* $Id: net.h,v 1.2 2001/05/31 21:32:22 marvin Exp $
 */
#ifndef __INCLUDE_NET_H__
#define __INCLUDE_NET_H__

/*
 * not nic, just protocols
 */
typedef struct net_driver_s {
	__uint32 magic;
	char *name;     /* "tcp/ip", "ipx" */
	
	char in_use;
	struct net_driver_s *next;
} net_driver_t;

#endif
