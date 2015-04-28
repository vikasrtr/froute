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
#include <net/ip_fib.h>
#include <net/flow.h>

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

int fib_rtable_lookup(struct sk_buff *skb, struct fib_route *fib_res, __be32 daddr, __be32 saddr, u8 tos) {
	//loop until a match
	struct fib_result res;
	struct flowi4 fl4;
	int		err = -EINVAL;
	struct net    *net = dev_net(skb->dev);
	struct fib_nh *nh;

	/*
	 *	Now we are ready to route packet.
	 */
	fl4.flowi4_oif = 0;
	fl4.flowi4_iif = skb->dev->ifindex;
	fl4.flowi4_mark = skb->mark;
	fl4.flowi4_tos = tos;
	fl4.flowi4_scope = RT_SCOPE_UNIVERSE;
	fl4.daddr = daddr;
	fl4.saddr = saddr;
	err = fib_lookup(net, &fl4, &res);
	if (err != 0) {
		return (-1);
	}
	if (res.type == RTN_LOCAL)
	{
		fib_res->type = RT_LOCAL;
	} else {
		fib_res->type = RT_OTHER;
	}
	fib_res->dev = FIB_RES_DEV(res);
	nh = &FIB_RES_NH(res);
	fib_res->ip = nh->nh_gw;

	return (0);

	/*	struct rl_head *rlist = rtable->next;
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
		return (-1);*/
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
