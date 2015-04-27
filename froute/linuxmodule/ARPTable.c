/*
 * ARPTable Implementation
 */
#include "ARPTable.h"

void init_arp_table(arp_entry *arptbl) {
	arptbl = 0;
}

void add_arp_entry(arp_entry *head, arp_entry *new_arp) {
	arp_entry *new;
	new = head;

	if (!new) {
		head = new_arp;
	}

	while(new->next != 0){
		new = new->next;
	}
	new->next = new_arp;
}

int arp_lookup(arp_entry *head, arp_entry *res, u32 ip) {
	arp_entry *new_h;
	new_h = head;

	while(new_h) {
		if (new_h->ip == ip) {
			res->ip = new_h->ip;
			memcpy(res->eth_addr, new_h->eth_addr, 6);
			return (0);
		}
		new_h = new_h->next;
	}
	return (-1);
}