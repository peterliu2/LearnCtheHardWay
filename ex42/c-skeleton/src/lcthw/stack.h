/**
 * @file stack.h
 * @brief stack header and macro
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-23
 */

#include <lcthw/list.h>

typedef List Stack;

#define Stack_create()		List_create()
#define Stack_destroy(S)	List_destroy(S)
#define Stack_push(S, V)	List_push(S, V)
#define Stack_peek(S)		List_last(S)
#define Stack_count(S)		List_count(S)
#define Stack_pop(S)		List_pop(S)

#define STACK_FOREACH(S, C)	ListNode *C = NULL;		\
for (C = S->last; C != NULL; C = C->prev)
