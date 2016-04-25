/**
 * @file list_tests.c
 * @brief list unit test
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-17
 */

#include "minunit.h"
#include <lcthw/dbg.h>
#include <lcthw/list.h>
#include <assert.h>

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

char *test_create(void)
{
	list = List_create();
	mu_assert(list != NULL, "Failed to create list.");
	return NULL;
}

char *test_destory(void)
{
	List_clear_destroy(list);
	return NULL;
}

char *test_push_pop(void)
{
	List_push(list, test1);
	mu_assert(List_last(list) == test1, "Wrong last value.");

	List_push(list, test2);
	mu_assert(List_last(list) == test2, "Wrong last value.");

	List_push(list, test3);
	mu_assert(List_last(list) == test3, "Wrong last value.");
	mu_assert(List_count(list) == 3, "Wrong list count on push.");

	char *val = List_pop(list);
	mu_assert(val == test3, "Wrong value on pop.");

	val = List_pop(list);
	mu_assert(val == test2, "Wrong value on pop.");

	val = List_pop(list);
	mu_assert(val == test1, "Wrong valu on pop.");
	mu_assert(List_count(list) == 0, "Wrong list count on pop.");

	return NULL;
}

char *test_unshift(void)
{
	List_unshift(list, test1);
	mu_assert(List_first(list) == test1, "Wrong first value.");

	List_unshift(list, test2);
	mu_assert(List_first(list) == test2, "Wrong first value.");

	List_unshift(list, test3);
	mu_assert(List_first(list) == test3, "Wrong first value.");
	mu_assert(List_count(list) == 3, "Wrong list count on unshift.");

	return NULL;
}

char *test_swep(void)
{
	List_swep(list->first, list->last);
	mu_assert(List_first(list) == test1, "Wrong swap on first.");
	mu_assert(List_last(list) == test3, "Wrong swap on last.");
	
	List_swep(list->last, list->first);
	mu_assert(List_first(list) == test3, "Wrong swap on first.");
	mu_assert(List_last(list) == test1, "Wrong swap in last.");

	List_swep(list->first, list->first);
	mu_assert(List_first(list) == test3, "Wrong swep on first after swep self.");

	return NULL;
}



char *test_remove(void)
{
	/* we only to test the middle remove case since pop/shift already tests the other cases */

	char *val = List_remove(list, list->first->next);
	mu_assert(val == test2, "Wrong remove value.");
	mu_assert(List_count(list) == 2, "Wrong list count after remove.");
	mu_assert(List_first(list) == test3, "Wrong first value after remove.");
	mu_assert(List_last(list) == test1, "Wrong last value after remove.");

	return NULL;
}

char *test_shift(void)
{
	mu_assert(List_count(list) == 2, "Wrong list count before shift start.");

	char *val = List_shift(list);
	mu_assert(val == test3, "Wrong first value on shift.");

	val = List_shift(list);
	mu_assert(val == test1, "Wrong first value on shift.");
	mu_assert(List_count(list) == 0, "Wrong list count after shift.");

	return NULL;
}

char *all_test(void)
{
	mu_suit_start();

	mu_run_test(test_create);
	mu_run_test(test_push_pop);
	mu_run_test(test_unshift);
	mu_run_test(test_swep);
	mu_run_test(test_remove);
	mu_run_test(test_shift);
	mu_run_test(test_destory);

	return NULL;
}

RUN_TESTS(all_test);
