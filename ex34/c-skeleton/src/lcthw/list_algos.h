/**
 * @file list_algos.h
 * @brief list algorithem Bubble and Merge header
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-17
 */
#ifndef _list_algos_h_
#define _list_algos_h_

#include <lcthw/list.h>

typedef int (*List_compare) (const void *a, const void *b);

int List_bubble_sort(List *list, List_compare compare);

List *List_merge_sort(List *list, List_compare compare);

#endif /* _list_algos_h_ */
