/**
 * @file stack_tests.c
 * @brief Stack unit test
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-23
 */

#include "minunit.h"
#include <lcthw/stack.h>
#include <assert.h>

static Stack *stack = NULL;
char *tests[] = { "test data 1","test data 2", "test data 3" };
#define NUM_TESTS 3

char *test_create(void)
{
	stack = Stack_create();
	mu_assert(stack != NULL, "Failed to create stack.");

	return NULL;
}

char *test_destroy(void)
{
	mu_assert(stack != NULL, "Failed to make stack #2.");
	Stack_destroy(stack);

	return NULL;
}

char *test_push_pop(void)
{
	int i = 0;
	for (i = 0; i < NUM_TESTS; i++) {
		Stack_push(stack, tests[i]);
		mu_assert(Stack_peek(stack) == tests[i], "Wrong next value.");
	}

	mu_assert(Stack_count(stack) == NUM_TESTS, "Wrong count on push.");

	STACK_FOREACH(stack, cur) {
		debug("VAL: %s", (char *)cur->value);
	}

	for (i = NUM_TESTS - 1; i >= 0; i--) {
		char *val = Stack_pop(stack);
		mu_assert(val == tests[i], "Wrong value on pop.");
	}

	mu_assert(Stack_count(stack) == 0, "Wrong count on pop.");
	return NULL;
}

char *all_tests(void)
{
	mu_suit_start();

	mu_run_test(test_create);
	mu_run_test(test_push_pop);
	mu_run_test(test_destroy);

	return NULL;
}

RUN_TESTS(all_tests);
