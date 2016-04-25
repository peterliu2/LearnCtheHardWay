/**
 * @file list_algos.c
 * @brief List Bubble and Merge sort implement
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-17
 */

#include <assert.h>
#include "lcthw/list_algos.h"

int List_bubble_sort(List *list, List_compare compare)
{
	assert(list && compare);	

	int i = 0;
	for (i = 0; i < List_count(list) - 1; i++) {
		LIST_FOREACH(list, first, next, cur) {
			if (cur->next && (compare(cur->value, cur->next->value) > 0))
				List_swep(cur, cur->next);				
		}
	}

	return 0;
}

static List *List_merge_sort_merge(List *list_1, List *list_2, List_compare compare)
{
	assert(list_1 && list_2 && compare);
	assert(List_count(list_1) > 0 || List_count(list_2) > 0);

	List *res = List_create();

	void *value_1 = List_shift(list_1);
	void *value_2 = List_shift(list_2);

	while (value_1 || value_2) 
	{
		if (value_1 == NULL) {
			while(value_2) {
				List_push(res, value_2);
				value_2 = List_shift(list_2);
			}
		} else if (value_2 == NULL) {
			while(value_1) {
				List_push(res, value_1);
				value_1 = List_shift(list_1);
			}
		} else {
			if(compare(value_1, value_2) <= 0) {
				List_push(res, value_1);
				value_1 = List_shift(list_1);
			} else {
				List_push(res, value_2);
				value_2 = List_shift(list_2);
			}
		}
	}
	return res;
}

static List *List_merge_sort_recurise(List* list, List_compare compare)
{
	assert(list && compare);

	List *res = NULL;

	if (List_count(list) == 0) {
		res = List_create();
		return res;
	} else if (List_count(list) == 1) {
		res = List_create();
		List_push(res, List_pop(list));
		return res; 
	}

	List *list_1 = List_create();
	List *list_2 = List_create();

	int j = List_count(list) / 2;
	int i = 0;
	for (i = 0; i < j; i++)
		List_push(list_1, List_pop(list));

	j = List_count(list);
	for (i = 0; i < j; i++) 
		List_push(list_2, List_pop(list));

	List *list_r1 = List_merge_sort_recurise(list_1, compare);
	List *list_r2 = List_merge_sort_recurise(list_2, compare);		

	List_destroy(list_1);
	List_destroy(list_2);
	
	res = List_merge_sort_merge(list_r1, list_r2, compare);

	List_destroy(list_r1);
	List_destroy(list_r2);

	return res;
}

List *List_merge_sort(List *list, List_compare compare)
{
	assert(list && compare);

	return List_merge_sort_recurise(list, compare);
}
