/**
 * @file ex23.c
 * @brief ex23
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-09
 */

#include <stdio.h>
#include <string.h>
#include "../common/include/dbg.h"

int normal_copy(char *from, char *to, int count)
{
	int i = 0;

	for (i = 0; i < count; i++)
		to[i] = from[i];

	return i;
}

int duffs_device(char *from, char *to, int count)
{
	{
		int n = (count + 7) / 8;

		switch (count % 8) {
		case 0:
			do {
				*to++ = *from++;

		case 7:	*to++ = *from++;
		case 6:	*to++ = *from++;
		case 5:	*to++ = *from++;
		case 4:	*to++ = *from++;
		case 3:	*to++ = *from++;
		case 2:	*to++ = *from++;
		case 1:	*to++ = *from++;
			} while (--n > 0);
		}
	}
	return count;
}

int zedx_device(char *from, char *to, int count)
{
	{
		int n = (count + 7) / 8;

		switch (count % 8) {
		case 0:
		again:	*to++ = *from++;

		case 7:	*to++ = *from++;
		case 6:	*to++ = *from++;
		case 5:	*to++ = *from++;
		case 4:	*to++ = *from++;
		case 3:	*to++ = *from++;
		case 2:	*to++ = *from++;
		case 1:	*to++ = *from++;
			if (--n > 0) goto again;
		}
	}
	return count;
}

int vaild_copy(char *data, int count, char expects)
{
	int i = 0;

	for (i = 0; i < count; i++) {
		if (data[i] != expects) {
			Log_info("[%d] %c != %c", i, data[i], expects);
			return 0;
		}
	}

	return 1;
}

int main(int argc, char **argv)
{
	char from[1000] = { 'a' };
	char to[1000] = { 'c' };
	int rc = 0;

	/**< setup from to have the same stuff */
	memset(from, 'x', 1000);

	/**< set it to a failure mode */
	memset(to, 'y', 1000);
	check(vaild_copy(to, 1000, 'y'), "Not initialized right.");

	/**< use normal copy to */
	rc = normal_copy(from, to, 1000);
	check(rc == 1000, "Normal copy failed: %d", rc);
	check(vaild_copy(to, 1000, 'x'), "Normal cpoy failed.");

	/**< reset */
	memset(to, 'y', 1000);

	/**< duffs version */
	rc = duffs_device(from, to, 1000);
	check(rc == 1000, "Duff's device failed: %d", rc);
	check(vaild_copy(to, 1000, 'x'), "Duff's device failed.");

	/**< reset */
	memset(to, 'y', 1000);

	/**< my version */
	rc = zedx_device(from, to, 1000);
	check(rc == 1000, "Zed's device failed: %d", rc);
	check(vaild_copy(to, 1000, 'x'), "Zed's device failed.");

	return 0;

error:
	return 1;
}
