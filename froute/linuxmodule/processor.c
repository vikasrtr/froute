/*
 * processor.h
 * main procedure to process packets available in queue
 */

#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <net/checksum.h>

#include "frouter.h"
#include "route.h"
#include "scheduler.h"


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
	struct fib_route res;
	struct iphdr *iph;
	struct FR_RX_Q *rx_q = &__get_cpu_var(fr_rx_q);

	do {
		if (fr_q_is_empty(rx_q)){
			return (__pkt_processed);
		}


		/* get a packet */
		fr_q_get(rx_q, skb, FR_RX_Q_SIZE);

		/* send promisc mode packets back to Kernel */
		if (skb->pkt_type == PACKET_OTHERHOST)
			fr_forward_skb_tohost(skb);
		

		/* find a IP header */
		if (!pskb_may_pull(skb, sizeof(struct iphdr)))
			goto inhdr_error;

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


		/* check for route */
		err = fib_rtable_lookup(main_rt_table, &res, (unsigned long)iph->daddr);
		if (err != 0)
			goto routing_error;
		
		/* deliver LOCAL packet to kernel */
		if (res.type == RT_LOCAL) {
			fr_forward_skb_tohost(skb);
		}

		/* TODO: Anything about broadcast rules ? */

		/* TODO: Check for REDIRECT */

		/* forward other packet */
		if (res.type == RT_OTHER) {
			/* Update IP fields */
			
			/* Decrease TTL after check*/
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
			skb->dev = res.dev;
						
			/* Update neighbour info */
			

			/* add to tx queue */
			fr_q_put(fr_tx_q, skb, FR_TX_Q_SIZE);

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
		printk(KERN_ALERT "#fr: Routing Error\n");

drop:
		kfree_skb(skb);
out:
		__pkt_processed += 1;
		if (__pkt_processed == _no_of_pkts) {
			/* budget is over */
			return (__pkt_processed);
		}

	} while(1);
}

/*
 * process_tx
 */
int process_tx(int __burst, struct FR_TX_Q *fr_tx_q) {
	
	/*
	 * Take packets one-by-one and feed them to dev_queue_xmit()
	 */
	int __pkt_processed = 0;
	struct sk_buff *skb;

	do{

		if (fr_q_is_empty(fr_tx_q))	{
			return (__pkt_processed);
		}

		/* deque a skb */
		fr_q_get(fr_tx_q, skb, FR_TX_Q_SIZE);

		dev_queue_xmit(skb);

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
