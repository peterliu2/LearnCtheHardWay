/**
 * @file string_algos.c
 * @brief string algorithm implement
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-22
 */

#include <lcthw/string_algos.h>
#include <limits.h>

static inline void String_setup_skip_chars(size_t *skip_chars, const unsigned char *needle, ssize_t nlen)
{
	size_t i = 0;
	size_t last = nlen - 1;

	for (i = 0; i < UCHAR_MAX + 1; i++) {
		skip_chars[i] = nlen;
	}

	for (i = 0; i < last; i++) {
		skip_chars[needle[i]] = last - i;
	}
}

static inline const unsigned char *String_base_search(
		const unsigned char *haystack, ssize_t hlen, 
		const unsigned char *needle, ssize_t nlen,
		size_t *skip_chars)
{
	size_t i = 0;
	size_t last = nlen - 1;

	assert(haystack != NULL && "Given bad haystack to search.");
	assert(needle != NULL && "Given bad needle to search for.");

	check(nlen > 0, "nlen can't be <= 0.");
	check(hlen > 0, "hlen can't be <= 0.");

	while (hlen >= nlen) {
		for (i = last; haystack[i] == needle[i]; i--) {
			if (i == 0) {
				return haystack;
			}
		}
		hlen -= skip_chars[haystack[last]];
		haystack += skip_chars[haystack[last]];
	}

error: /* fall throught */
	return NULL;
}

int String_find(bstring in, bstring what)
{
	const unsigned char *found = NULL;

	const unsigned char *haystack = (const unsigned char *)bdata(in);
	ssize_t hlen = blength(in);
	const unsigned char *needle = (const unsigned char *)bdata(what);
	ssize_t nlen = blength(what);
	size_t skip_chars[UCHAR_MAX + 1] = {0};

	String_setup_skip_chars(skip_chars, needle, nlen);

	found = String_base_search(haystack, hlen, needle, nlen, skip_chars);

	return found != NULL ? found - haystack : -1;
}

StringScanner *StringScanner_create(bstring in)
{
	StringScanner *scanner = calloc(1, sizeof(StringScanner));
	check_mem(scanner);

	scanner->in = in;
	scanner->haystack = (const unsigned char *)bdata(in);
	scanner->hlen = blength(in);

	assert(scanner != NULL && "fuck");
	return scanner;

error:
	free(scanner);
	return NULL;
}

static inline void StringScanner_set_needle(StringScanner *scanner, bstring tofind)
{
	scanner->needle = (const unsigned char *)bdata(tofind);
	scanner->nlen = blength(tofind);

	String_setup_skip_chars(scanner->skip_chars, scanner->needle, scanner->nlen);
}

static inline void StringScanner_reset(StringScanner *scanner)
{
	scanner->haystack = (const unsigned char *)bdata(scanner->in);
	scanner->hlen = blength(scanner->in);
}

int StringScanner_scan(StringScanner *scanner, bstring tofind)
{
	const unsigned char *found = NULL;
	ssize_t found_at = 0;

	if (scanner->hlen <= 0) {
		StringScanner_reset(scanner);
		return -1;
	}

	if ((const unsigned char *)bdata(tofind) != scanner->needle) {
		StringScanner_set_needle(scanner, tofind);
	}

	found = String_base_search(
			scanner->haystack, scanner->hlen,
			scanner->needle, scanner->nlen,
			scanner->skip_chars);

	if (found) {
		found_at = found - (const unsigned char *)bdata(scanner->in);
		scanner->haystack = found + scanner->nlen;
		scanner->hlen -= found_at - scanner->nlen;
	} else {
		/* done, reset the setup */
		StringScanner_reset(scanner);
		found_at = -1;
	}
	return found_at;
}

void StringScanner_destroy(StringScanner *scanner)
{
	if (scanner) {
		free(scanner);
	}
}


