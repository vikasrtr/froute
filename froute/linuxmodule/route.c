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

#include "route.h"

void init_rtable(struct fib_rtable *rt_table) {
	//
}

int fib_rtable_lookup(struct fib_rtable *rtable, struct fib_route *rt);

int fib_rtable_insert(struct fib_rtable *rtable, struct fib_route *new_rt) {
	return (0);
}
int fib_rtable_delete(struct fib_rtable *rtable, struct fib_route *rt);
int fib_rtable_freetable(struct fib_rtable *rtable);

/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
