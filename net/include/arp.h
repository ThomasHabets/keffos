/* $Id: arp.h,v 1.2 2001/05/31 21:32:23 marvin Exp $
 */
#ifndef __INCLUDE_NET_ARP_H__
#define __INCLUDE_NET_ARP_H__

#define MAX_PRA_SIZE 14 // FIXME: just a guess

#define ARP_TABLE_SIZE 128

typedef struct arp_packet_s {
	i16  ar_hwtype;
	i16  ar_prtype;
	char ar_hwlen;
	char ar_prlen;
	i16  ar_op;
	char ar_addrs[1];
} arp_packet_t;
  

enum arp_state_e {
	AS_FREE,
	AS_PENDING,
	AS_RESOLVED
};

typedef struct arp_table_entry_s {
	enum arp_state_e state;
	i16 hwtype;
	i16 prtype;
	char hwlen;
	char prlen;
	netif_t *netif;
	int queue;
	int attempts;
	int ttl;                     // max 136 years, -1 = imortal
	char hwa[MAX_HWA_SIZE];
	char pra[MAX_PRA_SIZE];
} arp_table_entry_t;

/*
typedef struct arp_api_s {
  arp_table_entry_t *(*get_free_table_entry)(void);
  arp_table_entry_t *(*add_entry)(netif_t *iface, arp_packet_t *arp_packet);
  void (*arp_table_free)(arp_table_entry_t *which);
  void (*resend_pending)(void);
  void (*table_clean_time)(int timesincelast);
} arp_api_t;
*/

#endif
