/**
 * @file queue_tests.c
 * @brief Queue unit test
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-23
 */

#include "minunit.h"
#include <lcthw/queue.h>
#include <assert.h>

static Queue *queue = NULL;
char *tests[] = { "test data 1","test data 2", "test data 3" };
#define NUM_TESTS 3

char *test_create(void)
{
	queue = Queue_create();
	mu_assert(queue != NULL, "Failed to create queue.");

	return NULL;
}

char *test_destroy(void)
{
	mu_assert(queue != NULL, "Failed to make queue #2.");
	Queue_destroy(queue);

	return NULL;
}

char *test_send_recv(void)
{
	int i = 0;
	for (i = 0; i < NUM_TESTS; i++) {
		Queue_send(queue, tests[i]);
		mu_assert(Queue_peek(queue) == tests[0], "Wrong next value.");
	}

	mu_assert(Queue_count(queue) == NUM_TESTS, "Wrong count on send.");

	QUEUE_FOREACH(queue, cur) {
		debug("VAL: %s", (char *)cur->value);
	}

	for (i = 0; i < NUM_TESTS; i++) {
		char *val = Queue_recv(queue);
		mu_assert(val == tests[i], "Wrong value on recv.");
	}

	mu_assert(Queue_count(queue) == 0, "Wrong count on recv.");
	return NULL;
}

char *all_tests(void)
{
	mu_suit_start();

	mu_run_test(test_create);
	mu_run_test(test_send_recv);
	mu_run_test(test_destroy);

	return NULL;
}

RUN_TESTS(all_tests);
