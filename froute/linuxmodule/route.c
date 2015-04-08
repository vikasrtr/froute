/* routing table implementation */

/*
 *================================================================================
 *
 *------------------- TEMPORARY IMPLEMENTATION -----------------------------------
 *
 *================================================================================
 */

// simply create linked list and iterate over iterate
//NOTE: API does NOT CHANGE
/*
 * route.c
 *
 * created: Feb 24, 15
 *
 * Routing table implementation
 *
 * @fib_rtable_lookup
 * @fib_rtable_insert
 * @fib_rtable_delete
 * @fib_rtable_dumpall/flush
 * @fib_rtable_freetable
 *
 */

#include <linux/slab.h>
#include <linux/string.h>

#include "route.h"
#include "frouter.h"

void init_rtable(struct fib_rtable *rt_table) {
	if (DEBUG)
	{
		printk(KERN_ALERT "Entering %s", __FUNCTION__);
	}
	rt_table = 0;
}

inline int processRoutes(struct fib_route *res, struct fib_route *first) {
	//check for single or multi routes: Algo here
	//for now
	res = first;
	return (0);
}

int fib_rtable_lookup(struct fib_rtable *rtable, struct fib_route *rt, unsigned int ip) {
	//loop until a match
	struct rl_head *rlist = rtable->next;
	if (!rlist)
	{
		rt = 0;
		return (0);
	}
	while (rlist) {
		if ((rlist->data->ip & rlist->data->mask) == (ip & rlist->data->mask))
		{
			//a match
			return processRoutes(rt, rlist->data);
		}
		rlist = rlist->next;
	}
	rt = 0;
	return (-1);
}

int fib_rtable_insert(struct fib_rtable *rtable, struct fib_route *new_rt) {
	//trvaverse link list and add; check for same route
	struct rl_head *bck_rt, *rt = rtable->next;
	struct rl_head *new_head;

	//initial case
	if (!rt)
	{
		bck_rt = kmalloc(sizeof(struct rl_head), GFP_KERNEL);

		if (!bck_rt)
		{
			printk(KERN_ALERT"fr# failed at kmalloc");
			return (-1);
		}

		bck_rt->next = 0;
		bck_rt->data = new_rt;
		return (0);
	}

	while (rt) {
		bck_rt = rt;

		//check same route
		if ((rt->data->ip & rt->data->mask ) == (new_rt->ip & new_rt->mask))
		{
			//check metric
			if (rt->data->metric == new_rt->metric)
			{
				//check for different devices
				if (!strcmp(rt->data->dev->name, new_rt->dev->name))
				{
					rt->data->next = new_rt;
				}
				return (0);

			} else {
				//multiple  metrics; ensure different nh
				if (rt->data->nh != new_rt->nh)
				{
					rt->data->next = new_rt;
				}
				return (0);
			}
		}

		rt = rt->next;
	}

	new_head = kmalloc(sizeof(struct rl_head), GFP_KERNEL);

	if (!new_head)
	{
		printk(KERN_ALERT"fr# failed at kmalloc");
		return (-1);
	}

	new_head->data = new_rt;
	new_head->next = 0;
	bck_rt = new_head;
	return (0);
}

int fib_rtable_delete(struct fib_rtable *rtable, struct fib_route *rt) {
	//temp jugad
	struct rl_head *rlist = rtable->next;

	if (!rlist)
	{
		return (0);
	}
	while (rlist) {
		if ((rlist->data->ip == rt->ip) && (rlist->data->mask == rt->mask))
		{
			//a match; try match all
			struct fib_route *temp1 = rlist->data;
			while (temp1) {
				if ((temp1->mask == rt->mask) && (temp1->nh == rt->nh) && (temp1->metric == rt->metric)) {
					temp1->ip = 0;
					temp1->mask = 32;
					return (0);
				}
				temp1 = temp1->next;
			}
			return (0);
		}
		rlist = rlist->next;
	}
	return (0);
}

int fib_rtable_freetable(struct fib_rtable *rtable) {
	//traverse all and free all
	struct rl_head *old_rlist, *rlist = rtable->next;

	while (rlist) {
		struct fib_route *old_rt, *rt = rlist->data;
		while (rt) {
			old_rt = rt;
			kfree(old_rt);
			rt = rt->next;
		}
		old_rlist = rlist;
		kfree(old_rlist);
		rlist = rlist->next;
	}
	kfree(rtable);
	return (0);
}

/*
 *
 *rlist
 *
 *
 *
 *
 *
 *
 *
 */
