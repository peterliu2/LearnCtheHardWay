/**
 * @file list_algos_tests.c
 * @brief Bubble and Merge sort algorithms
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-17
 */

#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <limits.h>

char *values[] = { "XXXX", "1234", "abcd", "xjvef", "NDSS", "rrfert", "fsgret", "43543", "fswrew", "32323" };
#define NUM_VALUES (sizeof(values)/sizeof(values[0]))

#define RAND_STRING_COUNT 10000
char *rand_string[RAND_STRING_COUNT];

#define RAND_STRING_LEN 100

static void gen_random_string(char *s, int len)
{
	static const char alphanum[] = 
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for(int i = 0; i < len - 1; i++) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	s[len - 1] = '\0';
}

List *create_words(void)
{
	unsigned int i = 0;

	List *words = List_create();

#if 0	
	for (i = 0; i < NUM_VALUES; i++)
		List_push(words, values[i]);
#else
	for (i = 0; i < RAND_STRING_COUNT; i++) {
		rand_string[i] = malloc(RAND_STRING_LEN);
		gen_random_string(rand_string[i], RAND_STRING_LEN);
		List_push(words, rand_string[i]);
	}
#endif
	return words;
}

int is_sorted(List *words)
{
	LIST_FOREACH(words, first, next, cur) {
		if (cur->next && strcmp(cur->value, cur->next->value) > 0) {
			debug("%s %s", (char *)cur->value, (char *)cur->next->value);
			return 0;
		}
	}

	return 1;
}

char *test_bubble_sort(void)
{
	List *words = create_words();

	// should work on a list that needs sorting
	int rc = List_bubble_sort(words, (List_compare)strcmp);
	mu_assert(rc == 0, "Bubble sort failed.");
	mu_assert(is_sorted(words), "Words should be sorted if already bubble sorted.");
	
	List_clear_destroy(words);

	// should work on an empty list
	words = List_create();
	rc = List_bubble_sort(words, (List_compare)strcmp);
	mu_assert(rc == 0, "Bubble sort failed on the empty list.");
	mu_assert(is_sorted(words), "Words should be sorted if empty.");

	List_destroy(words);

	return NULL;
}

char *test_merge_sort(void)
{
	List *words = create_words();

	// should work on a list that needs sorting
	List *res = List_merge_sort(words, (List_compare)strcmp);
	mu_assert(res, "Merge sort failed.");
	mu_assert(is_sorted(res), "words should be sorted if already merge sorted.");

	List *res2 = List_merge_sort(res, (List_compare)strcmp);
	mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
	
	List_clear_destroy(res);
	List_clear_destroy(res2);
	
	List_clear_destroy(words);
	return NULL;
}

char *all_test(void)
{
	mu_suit_start();
	
	clock_t start, end;

	start = clock();
	mu_run_test(test_bubble_sort);
	end = clock();
	printf("bubble sort time(sec): %lf\n", (end - start)*1.0/CLOCKS_PER_SEC);
	
	start = clock();
	mu_run_test(test_merge_sort);
	end = clock();
	printf("merge sort time(sec) : %lf\n", (end - start)*1.0/CLOCKS_PER_SEC);
	
	return NULL;
}

RUN_TESTS(all_test);
