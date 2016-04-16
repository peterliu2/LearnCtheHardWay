#include "../../../common/include/minunit.h"

char *test_dlopen(void)
{
	return NULL;
}

char *test_functions(void)
{
	return NULL;
}

char *test_failures(void)
{
	return NULL;
}

char *test_dlclose(void)
{
	return NULL;
}

char *all_tests(void)
{
	mu_suit_start();

	mu_run_test(test_dlopen);
	mu_run_test(test_functions);
	mu_run_test(test_failures);
	mu_run_test(test_dlclose);

	return NULL;
}

RUN_TESTS(all_tests);
