/**
 * froute software router Kernel Module
 *
 */

#include <linux/init.h>
#include <linux/module.h>

#include "route.h"

//some global vars
static int DEBUG = 0;

static struct fib_rtable *rt_table;

static int __init frouter_start(void) {

	printk(KERN_ALERT"#fr: Entering %s!\n", __FUNCTION__);

	/* Initialisation tasks */
	/**
	 * parse config
	 * initialise static vars
	 * initialise routing table
	 */
	DEBUG = 1;

	/* create routing tables */
	init_rtable(rt_table);

	/* start threads */

	return (0);
}

static void __exit frouter_shutdown(void) {

	printk(KERN_ALERT"#fr: Leaving module\n");
}

module_init( frouter_start);
module_exit( frouter_shutdown);

MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
MODULE_AUTHOR("vikas_rtr");
MODULE_DESCRIPTION("froute software-based IP router");
