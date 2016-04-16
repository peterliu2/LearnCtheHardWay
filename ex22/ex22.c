/**
 * @file ex22.c
 * @brief ex22
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-05
 */

#include "ex22.h"
#include "../common/include/dbg.h"

int THE_SIZE = 1000;

static int THE_AGE = 37;

int get_age(void)
{
	return THE_AGE;
}

void set_age(int age)
{
	THE_AGE = age;
}

double update_ratio(double new_ratio)
{
	static double ratio = 1.0;
	double old_ratio = ratio;

	ratio = new_ratio;
	return old_ratio;
}

void print_size(void)
{
	Log_info("I think size is %d.", THE_SIZE);
}
