/*
 * froute software router Kernel Module
 *
 * routines for scheduler and process threads
 *
 */
#include <linux/cpumask.h>
#include <linux/sched.h>
#include <linux/netdevice.h>
#include <linux/kthread.h>
#include <linux/smp.h>

#include "frouter.h"
#include "scheduler.h"

/*
 * fr_sched - the worker process
 */
int fr_sched(void *data)
{
	int _tx_burst, _no_of_pkts = 0;
	unsigned long _init_jiffies, work_done;

	int __netlink_pending = 0;

	/* Per-CPU Scheduling Data */
	struct router_sched rt_sched;

	/* Per-CPU Packet TX Queue */
	struct FR_TX_Q fr_tx_q;

	printk(KERN_ALERT "Froute: Printing from thread: %d\n", smp_processor_id());

	while (1) {
		sched_reset_budget(&rt_sched);
		// run tasks: netlink, packet_processor, os

		while (1) {
			work_done = 0;

			/* scheduling */
			if (sched_nl(&rt_sched))
				break;
			
			_init_jiffies = jiffies;
			if (__netlink_pending) {	//TODO: define __netlink_pending
				process_netlink();
			}

			work_done = jiffies - _init_jiffies;
			sched_nl_update_work(&rt_sched, work_done);
		}

		// process packets
		do{
			work_done = 0;

			/* scheduling */
			if (sched_packet(&rt_sched))
				break;

			_init_jiffies = jiffies;

			_no_of_pkts = 128;
			process_packets(_no_of_pkts, &fr_tx_q);

			work_done = jiffies - _init_jiffies;
			sched_packet_update_work(&rt_sched, work_done);

			/* check budget again */
			if (sched_packet(&rt_sched))
				break;

			_init_jiffies = jiffies;

			/* start tx */
			_tx_burst = 1024;
			process_tx(_tx_burst, &fr_tx_q);

			work_done = jiffies - _init_jiffies;
			sched_packet_update_work(&rt_sched, work_done);

		} while (1);

		// give control to OS
		while (1) {
			work_done = 0;

			/* scheduling */
			if (sched_os(&rt_sched))
				break;

			_init_jiffies = jiffies;

			// run the OS
			schedule_timeout(rt_sched.os_budget);

			work_done = jiffies - _init_jiffies;
			sched_os_update_work(&rt_sched, work_done);			
		}

		//check if thread has to stop
	}

	return (0);
}

/*
 * init_sched	-	start and schedule threads
 */
void init_sched(void)
{

	int cpus, i;

	if (DEBUG) {
		printk(KERN_INFO "entering func: %s\n", __FUNCTION__);
	}

	printk(KERN_INFO "starting worker threads...\n");

	cpus = num_online_cpus();
	for (i = 0; i < cpus; ++i) {
		struct task_struct *task = kthread_create(fr_sched, (void*)0, "kfrouter_%d", i);
		if (!task) {
			printk(KERN_ALERT "Froute: failed to create thread on cpu_no: %d\n", i);
			return;
		}
		kthread_bind(task, i);
		wake_up_process(task);
	}

	/* start the trap
	*/
	printk(KERN_ALERT "Froute: enabling FR_TRAP...\n");
	fr_trap_enable();
	printk(KERN_ALERT "Froute: FR_TRAP enabled...\n");

}