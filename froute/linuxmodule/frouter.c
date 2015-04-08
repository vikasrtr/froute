/**
 * froute software router Kernel Module
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/percpu.h>
#include <linux/netdevice.h>

#include "route.h"
#include "frouter.h"

//some global vars
int DEBUG = 1;
struct FR_RX_Q *rxq;

DEFINE_PER_CPU(struct task_struct, fr_task);
DEFINE_PER_CPU(struct fr_nl_data, fr_nl_data);

struct fib_rtable *main_rt_table;

static int __init frouter_start(void) {

/*	if (get_cpu_var(rx_q_switch)) {
		rx = &__get_cpu_var(fr_rx_q1);
	} else {
		rx = &__get_cpu_var(fr_rx_q0);
	}
	put_cpu_var(rx_q_switch);*/

	printk(KERN_ALERT "#fr: Entering %s!\n", __FUNCTION__);

	/* Initialisation tasks */
	/**
	 * parse config
	 * initialise static vars
	 * initialise routing table
	 */
	DEBUG = 1;

	/* create routing tables */
	init_rtable(main_rt_table);

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
	fib_rtable_freetable(main_rt_table);

/*	printk(KERN_ALERT "#fr: rx_queue has: %d\n", rxq->in);
*/
	fr_trap_disable();

	printk(KERN_ALERT "#fr: Leaving module\n");
}

module_init(frouter_start);
module_exit(frouter_shutdown);

MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
MODULE_AUTHOR("vikas_rtr");
MODULE_DESCRIPTION("froute software-based IP router");
