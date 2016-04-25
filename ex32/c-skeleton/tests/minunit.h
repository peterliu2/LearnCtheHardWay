/**
 * @file minunit.h
 * @brief minunit unit test
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-16
 */

#ifndef _minunit_h_
#define _minunit_h_

#include <stdio.h>
#include <lcthw/dbg.h>
#include <stdlib.h>

#define mu_suit_start() char *message = NULL

#define mu_assert(test, message) if (!(test)) { Log_err(message); return message; }
#define mu_run_test(test) debug("\n-----%s", " " #test); \
	message = test(); test_run++; if (message) return message;

#define RUN_TESTS(name) int main(int argc, char *argv[]) {	\
	(void)argc;												\
	argc = 1;												\
	printf("----- RUNNING: %s\n", argv[0]);					\
	char *result = name();									\
	if (result != 0) {										\
		printf("FAILED: %s\n", result);						\
	} else {												\
		printf("ALL TESTS PASSED\n");						\
	}														\
	printf("Tests run: %d\n", test_run);					\
	exit(result != 0);										\
}

int test_run;
#endif /*_minunit_h_*/
