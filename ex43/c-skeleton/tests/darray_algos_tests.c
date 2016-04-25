/**
 * @file darray_algos_tests.c
 * @brief Dynamic array algorithm test
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-19
 */

#include "minunit.h"
#include <lcthw/darray_algos.h>

static char * const words[] = { "asdfasfd", "werwar", "13234", "asdfasfd", "oioj", "asdfasfd", "werwar", "13234", "asdfasfd", "oioj"};

int testcmp(const void *a, const void *b)
{
	return strcmp(*(char * const *)a, *(char * const *)b);
}

DArray *create_words(void)
{
	DArray *result = DArray_create(0, sizeof(words)/sizeof(words[0]));

	size_t i = 0;

	for (i = 0; i < sizeof(words)/sizeof(words[0]); i++) {
		DArray_push(result, words[i]);
	}

	return result;
}

int is_sorted(DArray *array)
{
	int i = 0;

	for (i = 0; i < DArray_count(array) - 1; i++) {
		if (strcmp(DArray_get(array, i), DArray_get(array, i + 1)) > 0) {
			return 0;
		}
	}

	return 1;
}

char *run_sort_test(int (*func)(DArray *, DArray_compare), const char *name)
{
	DArray *words = create_words();
	mu_assert(!is_sorted(words), "Words should start not sorted.");

	debug("--- Testing \"%s\" sorting algorithm", name);
	int rc = func(words, (DArray_compare)testcmp);
	mu_assert(rc == 0, "sort failed.");
	mu_assert(is_sorted(words), "didn't sort it.");

	DArray_destroy(words);

	return NULL;
}

char *test_qsort(void)
{
	return run_sort_test(DArray_qsort, "qsort");
}

char *test_heapsort(void)
{
	return run_sort_test(DArray_heapsort, "heapsort");
}

char *test_mergesort(void)
{
	return run_sort_test(DArray_mergesort, "mergesort");
}

char *all_tests(void)
{
	mu_suit_start();

	mu_run_test(test_qsort);
	mu_run_test(test_heapsort);
	mu_run_test(test_mergesort);

	return NULL;
}

RUN_TESTS(all_tests);
