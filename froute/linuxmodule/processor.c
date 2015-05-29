/*
 * processor.h
 * main procedure to process packets available in queue
 */

#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <net/checksum.h>
#include <linux/if_ether.h>
#include <linux/string.h>

#include "frouter.h"
#include "route.h"
#include "scheduler.h"
#include "ARPTable.h"


/*
 * Packet Processing function
 */
int process_packets(int _no_of_pkts, struct FR_TX_Q *fr_tx_q) {

	//start by getting a packet from queue

	/*
	 * Steps of wisdom
	 * -- Find IP header
	 * -- Validate the Packet
	 * -- Check for route
	 * -- Check for Redirect
	 * -- If route matches, change the Params
	 * -- Update the neighbouring info (mac header)
	 * -- send the packet
	 */

	int err, __pkt_processed = 0;
	struct sk_buff *skb;
	struct fib_route fib_res;
	//arp_entry arp_res;
	struct ethhdr *eth;
	struct iphdr *iph;
	struct FR_RX_Q *rx_q = &__get_cpu_var(fr_rx_q);

	//printk(KERN_ALERT "==>\n");

	do {
		if (fr_q_is_empty(rx_q)) {
			printk(KERN_ALERT "\nrx_q EMPTY\n");
			return (__pkt_processed);
		}

		//printk(KERN_ALERT "\nPacket Info\n");


		/* get a packet */
		fr_q_get(rx_q, skb, FR_RX_Q_SIZE);

		/* send promisc mode packets back to Kernel */
		if (skb->pkt_type == PACKET_OTHERHOST) {
			printk(KERN_ALERT "\nFORWARD OTHER_HOST\n");
			fr_forward_skb_tohost(skb);
			continue;
		}

		/* find a IP header */
		if (!pskb_may_pull(skb, sizeof(struct iphdr)))
			goto inhdr_error;

		/*		//skb playground
				//printk(KERN_ALERT "head: %p\n", skb->head);
				//printk(KERN_ALERT "data: %p\n", skb->data);
				//printk(KERN_ALERT "data - head: %d\n", skb->data - skb->head);
				//printk(KERN_ALERT "mac_hdr: %d\n", skb->mac_header);
				//printk(KERN_ALERT "net_hdr: %d\n", skb->network_header);*/


		iph = ip_hdr(skb);

		/*
		 * Validation of IP Packet
		 * copied from Kernel
		*/
		if (iph->ihl < 5 || iph->version != 4)
			goto inhdr_error;

		if (unlikely(ip_fast_csum((u8 *)iph, iph->ihl)))
			goto csum_error;

		/* check if packet is for current system */

		////printk(KERN_ALERT "reached LOOKUP.");
		/*//printk(KERN_ALERT "Packet Src_IP: %lu\tOUT\n", (unsigned long)ntohl(ip_hdr(skb)->saddr));
		//printk(KERN_ALERT "Packet Dest_IP: %lu\tOUT\n", (unsigned long)ntohl(ip_hdr(skb)->daddr));
		*////goto drop;

		/* check for route */
		////printk(KERN_ALERT "ROUTE lookup:");
		err = fib_rtable_lookup(skb, &fib_res, iph->daddr, iph->saddr, iph->tos);

		//printk(KERN_ALERT "RT_Look info: nh_gw: %s\n", fib_res.dev->dev_addr);
		if (err != 0)
			goto routing_error;

		/* deliver LOCAL packet to kernel */
		if (fib_res.type == RT_LOCAL) {
			fr_forward_skb_tohost(skb);
		}

		/* TODO: Anything about broadcast rules ? */

		/* forward other packet */
		if (fib_res.type == RT_OTHER) {
			/* Update IP fields */

			/* Decrease TTL after check */
			if (iph->ttl <= 1) {
				goto ttl_fail;
			}
			ip_decrease_ttl(iph);

			/* Calculate checksum
			 * Ignoring here to see if it is done in hardware
			 */
			iph->check = 0;
			iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);


			/* Update output device */
			skb->dev = __dev_get_by_name(&init_net, "eth1");

			////printk(KERN_ALERT "reached arp_lookup..");
			/*			char *a = skb->dev->dev_addr;
						//printk(KERN_ALERT "dev_addr of eth1: %02x:%02x:%02x:%02x:%02x:%02x\n", a[0],a[1],a[2],a[3],a[4],a[5]);
						goto out;
			*/
			/* Update neighbour info */
			/* lookup in ARPTable */
			/*if (!arp_lookup(arp_tbl, &arp_res, fib_res.nh))
				goto arp_fail;
			*/
			/* get the Ethernet header */
			//skb_reset_mac_header(skb);
			eth = eth_hdr(skb);

			/* TODO: Check for REDIRECT */
			/*if (memcmp(eth->h_source, skb->dev->dev_addr, ETH_ALEN)){
				goto redirect_error;
			}*/
			char arp[ETH_ALEN] = {0x78, 0x84, 0x3c, 0xee, 0x6e, 0x08};
			//char arp[ETH_ALEN] = {0x12, 0x43, 0x84, 0x61, 0xf0, 0x91};

			char *a = skb->dev->dev_addr;
			//printk(KERN_ALERT "eth_H_source: %02x: %02x: %02x: %02x: %02x: %02x\n", a[0], a[1], a[2], a[3], a[4], a[5]);
			////printk(KERN_ALERT "eth_H_source: %d: %d: %d: %d: %d: %d\n", a[0], a[1], a[2], a[3], a[4], a[5]);

			/*			//printk(KERN_ALERT "Eth Header:[");
						int i;
						for (i = 0; i < 14; ++i)
						{
							//printk(KERN_ALERT "%0x", eth[i+2]);
						}
						//printk(KERN_ALERT "]<===");*/
			/* Update h_source and h_dest */
			memcpy(eth->h_source, skb->dev->dev_addr, ETH_ALEN);
			memcpy(eth->h_dest, arp, ETH_ALEN);
			skb->protocol = eth->h_proto = htons(ETH_P_IP);

			//a = eth->h_dest;
			////printk(KERN_ALERT "eth_H_dest: %02x: %02x: %02x: %02x: %02x: %02x\n", a[0], a[1], a[2], a[3], a[4], a[5]);

			/* add to tx queue */
			////printk(KERN_ALERT "Trying to add to tx_q");
			fr_q_put(fr_tx_q, skb, FR_TX_Q_SIZE);
			printk(KERN_ALERT "Added to tx_q\n");
		}
		goto out;

ttl_fail:
		printk(KERN_ALERT "#fr: IP TTL Expired. Send ICMP\n");
		goto drop;

csum_error:
		printk(KERN_ALERT "#fr: IP Header Checksum Error\n");
		goto drop;

inhdr_error:
		printk(KERN_ALERT "#fr: IP Header Error\n");
		goto drop;

routing_error:
		printk(KERN_ALERT "#fr: No Route Error\n");
		goto drop;

		/*arp_fail:
				//printk(KERN_ALERT "#fr: ARP Enntry not found\n");
		*/
drop:
		kfree_skb(skb);
out:
		__pkt_processed += 1;
		if (__pkt_processed == _no_of_pkts) {
			/* budget is over */
			return (__pkt_processed);
		}

	} while (1);
}

/*
 * process_tx
 */
int process_tx(int __burst, struct FR_TX_Q *fr_tx_q) {

	/*
	 * Take packets one-by-one and feed them to dev_queue_xmit()
	 */
	int __pkt_processed = 0, res;
	struct sk_buff *skb;

	do {

		if (fr_q_is_empty(fr_tx_q))	{
			printk(KERN_ALERT "\ntx_q EMPTY\n");
			return (__pkt_processed);
		}

		/* deque a skb */
		fr_q_get(fr_tx_q, skb, FR_TX_Q_SIZE);

		//skb playground
		/*		//printk(KERN_ALERT "head: %p\n", skb->head);
				//printk(KERN_ALERT "data: %p\n", skb->data);
				//printk(KERN_ALERT "data - head: %d\n", skb->data - skb->head);
				//printk(KERN_ALERT "mac_hdr: %d\n", skb->mac_header);
				//printk(KERN_ALERT "net_hdr: %d\n", skb->network_header);*/

		////printk(KERN_ALERT "Trying to xmit()\n");
		//skb->data = skb->data - 14;
		skb_push(skb, sizeof(struct ethhdr));
		skb_reset_mac_header(skb);
		res = dev_queue_xmit(skb);
		printk(KERN_ALERT "xmitted() with res: %d\n", res);

		__pkt_processed += 1;

		if (__pkt_processed == __burst) {
			return (__pkt_processed);
		}

	} while (1);
}

/*
 * Packet Processing function
 */
int process_netlink(void) {
	return (0);
}
