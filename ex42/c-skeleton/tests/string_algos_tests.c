/**
 * @file string_algos_tests.c
 * @brief String algorithm unut test
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-22
 */

#include "minunit.h"
#include <lcthw/string_algos.h>
#include <lcthw/bstrlib.h>
#include <time.h>

struct tagbstring IN_STR = bsStatic("I have ALPHA beta ALPHA and oranges ALPHA");
struct tagbstring ALPHA = bsStatic("ALPHA");
const int TEST_TIME = 9;

char *test_find_and_scan(void)
{
	StringScanner *scanner = StringScanner_create(&IN_STR);
	mu_assert(scanner != NULL, "Failed to make the scanner.");

	int find_i = String_find(&IN_STR, &ALPHA);
	mu_assert(find_i > 0, "Failed to find 'ALPHA' in test string.");

	int scan_i = StringScanner_scan(scanner, &ALPHA);
	mu_assert(scan_i > 0, "Failed to scan 'ALPHA' in test string.");
	mu_assert(scan_i == find_i, "find ans scan don't match.");

	scan_i = StringScanner_scan(scanner, &ALPHA);
	mu_assert(scan_i > find_i, "should find another ALPHA after the first.");

	scan_i = StringScanner_scan(scanner, &ALPHA);
	mu_assert(scan_i > find_i, "should find another ALPHA after the first.");

	mu_assert(StringScanner_scan(scanner, &ALPHA) == -1, "shouldn't find it.");

	StringScanner_destroy(scanner);
	return NULL;
}

char *test_binstr_performance(void)
{
	int i = 0;
	int found_at = 0;
	unsigned long find_count = 0;
	time_t elapsed = 0;
	time_t start = time(NULL);

	do {
		for (i = 0; i < 1000; i++) {
			found_at = binstr(&IN_STR, 0, &ALPHA);
			//mu_assert(found_at != BSTR_ERR, "Failed to find on binstr().");
			find_count++;
		}
		elapsed = time(NULL) - start;
	} while (elapsed <= TEST_TIME);

	debug("\tBINSTR COUNT: %lu,\tEND TIME: %d,\tOPS: %f", 
			find_count, (int)elapsed, (double)find_count/elapsed);
	return NULL;
}

char *test_find_performance(void)
{
	int i = 0;
	int found_at = 0;
	unsigned long find_count = 0;
	time_t elapsed = 0;
	time_t start = time(NULL);

	do {
		for (i = 0; i < 1000; i++) {
			found_at = String_find(&IN_STR, &ALPHA);
			find_count++;
		}
		elapsed = time(NULL) - start;
	} while (elapsed <= TEST_TIME);
	
	debug("\tFIND COUNT:   %lu,\t\tEND TIME: %d,\tOPS: %f", 
			find_count, (int)elapsed, (double)find_count/elapsed);
	return NULL;
}

char *test_scan_performance(void)
{
	int i = 0;
	int found_at = 0;
	unsigned long find_count = 0;
	time_t elapsed = 0;
	time_t start = time(NULL);
	
	StringScanner *scanner = StringScanner_create(&IN_STR);

	do {
		for (i = 0; i < 1000; i++) {
			found_at = 0;

			do {
				found_at = StringScanner_scan(scanner, &ALPHA);
				find_count++;
			} while (found_at != -1);
		}

		elapsed = time(NULL) - start;
	} while (elapsed <= TEST_TIME);
	
	debug("\tSACN COUNT:   %lu,\tEND TIME: %d,\tOPS: %f", 
			find_count, (int)elapsed, (double)find_count/elapsed);

	StringScanner_destroy(scanner);
	return NULL;
}

char *all_tests(void)
{
	mu_suit_start();

	mu_run_test(test_find_and_scan);

	/* this is an idiom for commenting out sections of code */
#if 0 	
	mu_run_test(test_scan_performance);
	mu_run_test(test_find_performance);
	mu_run_test(test_binstr_performance);
#endif

	return NULL;
}

RUN_TESTS(all_tests)
