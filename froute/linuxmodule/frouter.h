/*
 * frouter.h
 *
 *      Author: vikas
 */

#ifndef FROUTE_LINUXMODULE_FROUTER_H_
#define FROUTE_LINUXMODULE_FROUTER_H_

#include <linux/skbuff.h>

/*
 * DEBUG Flag
 */
extern int DEBUG;

/*
 * List of queue netlink skbs
 *
 * To be initialised per-cpu for optimal performance
 */
struct fr_nl_data
{
	struct sk_buff *q;
};

#define FR_TX_Q_SIZE 1024

/*
 * struct fr_tx_q
 *
 * holds info and data about outgoing packets
 * Uses same data functions as FR_RX_Q (see patched netdevice.h)
 */
struct FR_TX_Q
{
	struct sk_buff *pkt_q[FR_TX_Q_SIZE];
	unsigned int in;
	unsigned int out;
};

/*
 * Main Routing Table
 */
extern struct fib_rtable *main_rt_table;

/*
 * init_sched	-	start and schedule threads
 */
extern void init_sched(void);

/*
 * Misc utility functions
 */
static inline void fr_skb_reset_network_header(struct sk_buff *skb)
{
	skb->network_header = skb->data - skb->head;
}

#endif /* FROUTE_LINUXMODULE_FROUTER_H_ */