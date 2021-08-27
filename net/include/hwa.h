/* $Id: hwa.h,v 1.2 2001/05/31 21:32:23 marvin Exp $
 */
#ifndef __INCLUDE_NET_HWA_H__
#define __INCLUDE_NET_HWA_H__

#define MAX_HWA_SIZE 14 // FIXME: just a guess

typedef struct net_hwa_s {
	int len;
	char address[MAX_HWA_SIZE];
} net_hwa_t;

#endif
