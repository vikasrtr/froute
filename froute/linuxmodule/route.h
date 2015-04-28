/*
 * route.h
 *
 *  Created on: 24-Feb-2015
 *      Author: vikas
 */

#ifndef FROUTE_LINUXMODULE_ROUTE_H_
#define FROUTE_LINUXMODULE_ROUTE_H_

#include <linux/netdevice.h>

/*
 * route types
 */
enum rt_type {
	RT_LOCAL,	/* deliver locally */
	RT_OTHER	/* forward to other host */
};

struct rl_head {
	struct rl_head *next;
	struct fib_route *data;
};

struct fib_rtable {
	struct rl_head *next;
};

struct fib_route {
	u32 ip;
	u32 mask;
	u32 nh;
	unsigned int metric;
	struct net_device *dev;	/* Beware */
	enum rt_type type;	/* type of route */
	struct fib_route *next;
};

// protos
extern void init_rtable(struct fib_rtable *rt_table);
extern int fib_rtable_lookup(struct sk_buff *skb, struct fib_route *fib_res, __be32 daddr, __be32 saddr, u8 tos);
extern int fib_rtable_insert(struct fib_rtable *rtable, struct fib_route *new_rt);
extern int fib_rtable_delete(struct fib_rtable *rtable, struct fib_route *rt);
extern int fib_rtable_freetable(struct fib_rtable *rtable);

#endif /* FROUTE_LINUXMODULE_ROUTE_H_ */