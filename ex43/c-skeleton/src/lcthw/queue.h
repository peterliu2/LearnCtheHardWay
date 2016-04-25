/**
 * @file queue.h
 * @brief Queue header and macro
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-23
 */

#ifndef _queue_h_
#define _queue_h_

#include <lcthw/list.h>

typedef List Queue;

#define Queue_create()		List_create()
#define Queue_destroy(Q)	List_destroy(Q)
#define Queue_send(Q, V)	List_unshift(Q, V)
#define Queue_peek(Q)		List_last(Q)
#define Queue_count(Q)		List_count(Q)
#define Queue_recv(Q)		List_pop(Q)

#define QUEUE_FOREACH(Q, C) ListNode *C = NULL; \
for (C = Q->last; C != NULL; C = C->prev)

#endif /* _queue_h_  */
