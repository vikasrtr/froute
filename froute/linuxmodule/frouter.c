/**
 * froute software router Kernel Module
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/percpu.h>
#include <linux/netdevice.h>
#include <net/sock.h>

#include "route.h"
#include "frouter.h"
#include "ARPTable.h"

// NETLINK Starts
#define NETLINK_FR 31

//some global vars
int DEBUG = 1;
struct FR_RX_Q *rxq;

DEFINE_PER_CPU(struct task_struct, fr_task);
//DEFINE_PER_CPU(struct fr_nl_data, fr_nl_data);

struct fib_rtable *main_rt_table;
arp_entry *arp_tbl = 0;

struct sock *nl_sk = 0;

static void fr_nl_recv_msg(struct sk_buff *skb)
{
	// Handle here only
	
}

static int __init frouter_start(void) {

	/*
	 * Netlink Method
	 */
/*    struct netlink_kernel_cfg cfg =
    {
        .input = fr_nl_recv_msg,
    };

    nl_sk = netlink_kernel_create(&init_net, NETLINK_FR, &cfg);

    if (!nl_sk)
    {
        printk(KERN_ALERT "Error creating Netlink Socket !!\n");
        return (-10);
    }*/

	printk(KERN_ALERT "#fr: Entering %s!\n", __FUNCTION__);

	/* Initialisation tasks */
	/**
	 * parse config
	 * initialise static vars
	 * initialise routing table
	 */
	DEBUG = 1;

	/* create routing tables */
	//init_rtable(main_rt_table);

	/* create main ARP table */
	//init_arp_table(arp_tbl);

	/* start threads */
	init_sched();

	/*	printk(KERN_ALERT "#fr: rx_queue has: %d\n", rx->index);
		fr_trap_enable();
		printk(KERN_ALERT "#fr: TRAP enabled\n");
		printk(KERN_ALERT "#fr: rx_queue has: %d\n", rx->index);
	*/

	return (0);
}

static void __exit frouter_shutdown(void) {

	//free up routes
	//fib_rtable_freetable(main_rt_table);

/*	printk(KERN_ALERT "#fr: rx_queue has: %d\n", rxq->in);
*/
	fr_trap_disable();

	//Kill all threads here

	printk(KERN_ALERT "#fr: Leaving module!\n");
}

module_init(frouter_start);
module_exit(frouter_shutdown);

MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
MODULE_AUTHOR("vikas_rtr");
MODULE_DESCRIPTION("froute software-based IP router");
