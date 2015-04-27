/*
 * Implementatiion of a simple ARPTable using simple Linked List
 */

#ifndef FROUTE_LIB_ARPTABLE_H_
#define FROUTE_LIB_ARPTABLE_H_

#include <linux/string.h>

/*
 * ARP Entry
 */
struct arp_entry_s {
	u32 ip;
	unsigned char eth_addr[6];
	struct arp_entry_s *next;
};
typedef struct arp_entry_s arp_entry;

extern void init_arp_table(arp_entry *arptbl);

extern void add_arp_entry(arp_entry *head, arp_entry *new_arp);

extern int arp_lookup(arp_entry *head, arp_entry *res, u32 ip);

#endif /* FROUTE_LIB_ARPTABLE_H_ */