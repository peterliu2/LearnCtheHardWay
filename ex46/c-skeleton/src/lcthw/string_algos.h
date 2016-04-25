/**
 * @file string_algos.h
 * @brief String algorithm header
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-21
 */

#ifndef _string_algos_h_
#define _string_algos_h_

#include <lcthw/bstrlib.h>
#include <lcthw/darray.h>

typedef struct StringScanner {
	bstring in;
	const unsigned char *haystack;
	ssize_t hlen;
	const unsigned char *needle;
	ssize_t nlen;
	size_t skip_chars[UCHAR_MAX + 1];
} StringScanner;

int String_find(bstring in, bstring what);
StringScanner *StringScanner_create(bstring in);
int StringScanner_scan(StringScanner *scanner, bstring tofind);
void StringScanner_destroy(StringScanner *scanner);

#endif /* _string_algos_h_ */
