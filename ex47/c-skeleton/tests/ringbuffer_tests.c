/**
 * @file ringbuffer_tests.c
 * @brief Ring buffer unit test
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-24
 */

#include "minunit.h"
#include <lcthw/ringbuffer.h>
#include <lcthw/dbg.h>
#include <lcthw/bstrlib.h>
#include <string.h>

#define RINGBUFFER_LEN	10

RingBuffer *b = NULL;
const char test[] = "1234567890";


char *test_create(void)
{
	b = RingBuffer_create(RINGBUFFER_LEN);
	mu_assert(b != NULL, "Failed to create ring buffer.");
	return NULL;
}

char *test_destroy(void)
{
	RingBuffer_destroy(b);
	return NULL;
}


char *test_write_read(void) 
{
	char temp[sizeof(test)] = {0};
	int rc;
	// 1 byte write read
	rc = RingBuffer_write(b, (char *)test, 1);
	mu_assert(rc != -1, "Failed to write ring buffer.");
	mu_assert(rc == 1, "Wrong write return length.");
	mu_assert(RingBuffer_available_data(b) == 1, "Wrong available data.");
	mu_assert(RingBuffer_available_space(b) == RINGBUFFER_LEN - 1, "Wrong available space.");
	mu_assert(!RingBuffer_full(b), "Shouldn't be full.");
	mu_assert(!RingBuffer_empty(b), "Shouldn't be empty.");

	rc = RingBuffer_read(b, temp, 1);
	mu_assert(rc != -1, "Failed to read ring buffer.");
	mu_assert(rc == 1, "Wrong read return length.");
	mu_assert(strncmp(test, temp, 1) == 0, "Wrong read content.content");
	mu_assert(RingBuffer_available_data(b) == 0, "Wrong available data.");
	mu_assert(RingBuffer_available_space(b) == RINGBUFFER_LEN, "Wrong available space.");
	mu_assert(!RingBuffer_full(b), "Shouldn't be full.");
	mu_assert(RingBuffer_empty(b), "Should be empty.");

	// (RINGBUFFER_LEN - 1) bytes write read
	rc = RingBuffer_write(b, (char *)test, RINGBUFFER_LEN-1);
	mu_assert(rc != -1, "Failed to write ring buffer.");
	mu_assert(rc == RINGBUFFER_LEN-1, "Wrong write return length.");
	mu_assert(RingBuffer_available_data(b) == RINGBUFFER_LEN-1, "Wrong available data.");
	mu_assert(RingBuffer_available_space(b) == 1, "Wrong available space.");
	mu_assert(!RingBuffer_full(b), "Shouldn't be full.");
	mu_assert(!RingBuffer_empty(b), "Shouldn't be empty.");

	rc = RingBuffer_read(b, temp, RINGBUFFER_LEN-1);
	mu_assert(rc != -1, "Failed to read ring buffer.");
	mu_assert(rc == RINGBUFFER_LEN-1, "Wrong read return length.");
	mu_assert(strncmp(test, temp, RINGBUFFER_LEN-1) == 0, "Wrong read content.content");
	mu_assert(RingBuffer_available_data(b) == 0, "Wrong available data.");
	mu_assert(RingBuffer_available_space(b) == RINGBUFFER_LEN, "Wrong available space.");
	mu_assert(!RingBuffer_full(b), "Shouldn't be full.");
	mu_assert(RingBuffer_empty(b), "Should be empty.");

	// (RINGBUFFER_LEN) bytes write read
	rc = RingBuffer_write(b, (char *)test, RINGBUFFER_LEN);
	mu_assert(rc != -1, "Failed to write ring buffer.");
	mu_assert(rc == RINGBUFFER_LEN, "Wrong write return length.");
	mu_assert(RingBuffer_available_data(b) == RINGBUFFER_LEN, "Wrong available data.");
	mu_assert(RingBuffer_available_space(b) == 0, "Wrong available space.");
	mu_assert(RingBuffer_full(b), "Should be full.");
	mu_assert(!RingBuffer_empty(b), "Shouldn't be empty.");

	rc = RingBuffer_read(b, temp, RINGBUFFER_LEN);
	mu_assert(rc != -1, "Failed to read ring buffer.");
	mu_assert(rc == RINGBUFFER_LEN, "Wrong read return length.");
	mu_assert(strncmp(test, temp, RINGBUFFER_LEN) == 0, "Wrong read content.content");
	mu_assert(RingBuffer_available_data(b) == 0, "Wrong available data.");
	mu_assert(RingBuffer_available_space(b) == RINGBUFFER_LEN, "Wrong available space.");
	mu_assert(!RingBuffer_full(b), "Shouldn't be full.");
	mu_assert(RingBuffer_empty(b), "Should be empty.");

	// (RINGBUFFER_LEN+1) bytes write read
#if 1
	rc = RingBuffer_write(b, (char *)test, RINGBUFFER_LEN + 1);
	mu_assert(rc == -1, "Failed to over-write ring buffer.");
	mu_assert(RingBuffer_available_data(b) == 0, "Wrong available data.");
	mu_assert(RingBuffer_available_space(b) == RINGBUFFER_LEN, "Wrong available space.");
	mu_assert(!RingBuffer_full(b), "Shouldn't be full.");
	mu_assert(RingBuffer_empty(b), "Should be empty.");
#endif
	return NULL;
}

char *test_put_get(void)
{
	int rc = 0;

	bstring s = bfromcstr(test);
	rc = RingBuffer_puts(b, s);
	mu_assert(rc != -1, "Failed to puts ring buffer.");
	mu_assert(rc == blength(s), "Wrong puts return length.");
	mu_assert(RingBuffer_available_data(b) == blength(s), "Wrong available data.");
	mu_assert(RingBuffer_available_space(b) == RINGBUFFER_LEN - blength(s), "Wrong available space.");
	mu_assert(RingBuffer_full(b), "Should be full.");
	mu_assert(!RingBuffer_empty(b), "Shouldn't be empty.");

	bstring s_gets = RingBuffer_get_all(b);
	mu_assert(s_gets != NULL, "Failed to gets.");
	mu_assert(bstrcmp(s, s_gets) == 0, "Failed to get all.");

	bdestroy(s);
	bdestroy(s_gets);
	return NULL;
}

char *all_tests(void)
{
	mu_suit_start();

	mu_run_test(test_create);
	mu_run_test(test_write_read);
	mu_run_test(test_put_get);
	mu_run_test(test_destroy);
	return NULL;
}

RUN_TESTS(all_tests)
