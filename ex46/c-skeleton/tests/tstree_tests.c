/**
 * @file tstree_tests.c
 * @brief Ternary Search Tree unit test
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-25
 */

#include "minunit.h"
#include <lcthw/tstree.h>
#include <string.h>
#include <assert.h>
#include <lcthw/bstrlib.h>

TSTree *node = NULL;
char *value_A = "VALUEA";
char *value_B = "VALUEB";
char *value_2 = "VALUE2";
char *value_4 = "VALUE4";
char *reverse = "VALUER";
int traverse_count = 0;

struct tagbstring test_1 = bsStatic("TEST");
struct tagbstring test_2 = bsStatic("TEST2");
struct tagbstring test_3 = bsStatic("TSET");
struct tagbstring test_4 = bsStatic("T");

char *test_insert(void)
{
	node = TSTree_insert(node, bdata(&test_1), blength(&test_1), value_A);
	mu_assert(node != NULL, "Failed to insert into tst with first name.");
	
	node = TSTree_insert(node, bdata(&test_2), blength(&test_2), value_2);
	mu_assert(node != NULL, "Failed to insert into tst with second name.");
	
	node = TSTree_insert(node, bdata(&test_3), blength(&test_3), reverse);
	mu_assert(node != NULL, "Failed to insert into tst with reverse name.");
	
	node = TSTree_insert(node, bdata(&test_4), blength(&test_4), value_4);
	mu_assert(node != NULL, "Failed to insert into tst with forth name.");

	return NULL;
}

char *test_search_exact(void)
{
	/* tst returns the last one inserted */
	void *res = TSTree_search(node, bdata(&test_1), blength(&test_1));
	mu_assert(res == value_A, "Got the wrong value back, should get A not 2.");

	/* tst dosen't find if not exact */
	res = TSTree_search(node, "TESTNO", strlen("TESTNO"));
	mu_assert(res == NULL, "Should not find anything.");
		
	return NULL;
}

char *test_search_prefix(void)
{
	void *res = TSTree_search_prefix(node, bdata(&test_1), blength(&test_1));
	debug("result: %p, expect: %p", res, value_A);
	mu_assert(res == value_A, "Got wrong value_A prefix.");

	res = TSTree_search_prefix(node, bdata(&test_1), 1);
	debug("result: %p, expect: %p", res, value_4);
	mu_assert(res == value_4, "Got wrong value_4 prefix of 1.");

	res = TSTree_search_prefix(node, "TE", strlen("TE"));
	debug("result: %p, expect: %p", res, value_A);
	mu_assert(res == value_A, "Should find for short prefix.");

	res = TSTree_search_prefix(node, "TE--", strlen("TE--"));
	debug("result: %p, expect: %p", res, value_4);
	mu_assert(res == value_4, "Should find for partial prefix.");
	
	return NULL;
}

static void TSTree_traverse_test_cd(void *value, void *data)
{
	assert(value != NULL && "Should not get NULL value.");
	assert(data == value_A && "Expecting value_A as the data.");
	traverse_count++;
}

char *test_traverse(void)
{
	traverse_count = 0;
	TSTree_traverse(node, TSTree_traverse_test_cd, value_A);
	debug("traverse count is: %d", traverse_count);
	mu_assert(traverse_count == 4, "Didn't find 4 keys.");
	return NULL;
}

char *test_destroy(void)
{
	TSTree_destroy(node);
	return NULL;
}

char * all_tests(void)
{
	mu_suit_start();

	mu_run_test(test_insert);
	mu_run_test(test_search_exact);
	mu_run_test(test_search_prefix);
	mu_run_test(test_traverse);
	mu_run_test(test_destroy);

	return NULL;
}

RUN_TESTS(all_tests);
