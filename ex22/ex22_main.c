/**
 * @file ex22_main.c
 * @brief ex22 main
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-09
 */

#include "ex22.h"
#include "../common/include/dbg.h"

const char *MY_NAME = "peter liu";

void scope_demo(int count)
{
	Log_info("count is: %d", count);

	if (count > 10) {
		int count = 100;	/**< BAD! BUGS! */

		Log_info("count in this scope is: %d", count);
	}

	Log_info("count at exit is: %d", count);

	count = 3000;
	Log_info("count after assigment is: %d", count);
}

int main(int agrc, char **agrv)
{
    /**< test out THE_AGE accessors*/
	Log_info("My name: %s, age: %d", MY_NAME, get_age());

	set_age(100);
	Log_info("My age now is: %d", get_age());

    /**< test the extern THE_SIZE */
	Log_info("THE_SIZE is: %d", THE_SIZE);
	print_size();

	THE_SIZE = 9;

	Log_info("THE_SIZE now is: %d", THE_SIZE);
	print_size();

    /**< test the ration function static */
	Log_info("Ratio at first: %f", update_ratio(2.0));
	Log_info("Ratio again: %f", update_ratio(10.0));
	Log_info("Ratio once more: %f", update_ratio(300.0));

    /**< test scope_demo */
	int count = 4;

	scope_demo(count);
	scope_demo(count * 20);

	Log_info("count after calling scope_demo: %d", count);

	return 0;
}

