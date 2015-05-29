/*
 * Scheduler Functions
 *
 * schedules CPU share for Netlink, Packet Processor, Packet Forwarder
 */

#ifndef FROUTE_LINUXMODULE_SCHEDULER_H_
#define FROUTE_LINUXMODULE_SCHEDULER_H_

#include <linux/param.h>
#include <linux/jiffies.h>

#define NETLINK_MAX_BUDGET (0.1 * HZ)
#define ROUTER_MAX_BUDGET (0.5 * HZ)
#define OS_MAX_BUDGET (1 * HZ)

struct router_sched {

	/* Netlink CPU Cycles */
	unsigned long netlink_budget;// = NETLINK_MAX_BUDGET;

	/* Packet Processor Cycles */
	unsigned long router_budget;// = ROUTER_MAX_BUDGET;

	/* OS Processor Cycles */
	unsigned long os_budget;// = OS_MAX_BUDGET;

};

/*
 * sched_netlink
 *
 * schedules process_netlink function
 *
 * return 0 - if budget remains
 * return 1 - if budget over
 */
inline static int sched_nl(struct  router_sched *rt_struct)
{
	if (rt_struct->netlink_budget > 0)
	{
		return (0);
	} else	{
		return (1);
	}
};

/*
 * sched_packet
 *
 * schedules process_packets function
 *
 * return 0 - if budget remains
 * return 1 - if budget over
 */
inline static int sched_packet(struct  router_sched *rt_struct)
{
	//printk(KERN_ALERT "sched_packet RT_BUDGET: %lu\n", rt_struct->router_budget);
	if (rt_struct->router_budget > 0) {
		return (0);
	} else	{
		return (1);
	}
};

/*
 * sched_os
 *
 * schedules OS for CPU
 *
 * return 0 - if budget remains
 * return 1 - if budget over
 */
inline static int sched_os(struct  router_sched *rt_struct)
{
	//printk(KERN_ALERT "sched_packet OS_BUDGET: %lu\n", rt_struct->os_budget);
	if (rt_struct->os_budget > 0) {
		return (0);
	} else	{
		return (1);
	}
};

/*
 * sched_nl_update_work
 *
 * updates workdone by a call
 */
inline static void sched_nl_update_work(struct  router_sched *rt_struct, unsigned long work_done)
{
	rt_struct->netlink_budget -= work_done;
};

/*
 * sched_packet_update_work
 *
 * updates workdone by a call
 */
inline static void sched_packet_update_work(struct  router_sched *rt_struct, unsigned long work_done)
{
	//printk(KERN_ALERT "RT_UPDATE_BUDGET: Work/Total => %lu/%lu\n", work_done, rt_struct->router_budget);
	if (work_done <= rt_struct->router_budget) {
		rt_struct->router_budget -= work_done;
	} else {
		rt_struct->router_budget = 0;
	}
};

/*
 * sched_os_update_work
 *
 * updates workdone by a call
 */
inline static void sched_os_update_work(struct  router_sched *rt_struct, unsigned long work_done)
{
	//printk(KERN_ALERT "OS_UPDATE_BUDGET: Work/Total => %lu/%lu\n", work_done, rt_struct->os_budget);
	if (work_done <= rt_struct->os_budget) {
		rt_struct->os_budget -= work_done;
	} else {
		rt_struct->os_budget = 0;
	}
};

/*
 * sched_reset_budget
 *
 * updates workdone by a call
 */
inline static void sched_reset_budget(struct  router_sched *rt_struct)
{
	printk(KERN_ALERT "Resetting Budget at HZ:%d\n", HZ);
	rt_struct->netlink_budget = NETLINK_MAX_BUDGET;
	rt_struct->router_budget = ROUTER_MAX_BUDGET;
	rt_struct->os_budget = OS_MAX_BUDGET;
};
/*
inline static void reset_netlink_budget(struct router_sched *rt_struct) {
	rt_struct->netlink_budget = NETLINK_MAX_BUDGET;
}

inline static void reset_router_budget(struct router_sched *rt_struct) {
	rt_struct->router_budget = ROUTER_MAX_BUDGET;
}

inline static void reset_os_budget(struct router_sched *rt_struct) {
	rt_struct->os_budget = OS_MAX_BUDGET;
}*/

/*
 * process_packets	-	main process for processing packets
 */
extern int process_packets(int, struct FR_TX_Q *);

/*
 * process_netlink	-	process netlink queue packets
 */
extern int process_netlink(void);

/*
 * process_tx	-	process TX queue packets
 */
extern int process_tx(int, struct FR_TX_Q *);

#endif /* FROUTE_LINUXMODULE_SCHEDULER_H_ */