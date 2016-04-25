/**
 * @file darray.c
 * @brief Dynamic array implement
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-18
 */

#include <lcthw/darray.h>
#include <assert.h>

DArray *DArray_create(size_t element_size, size_t initial_max)
{
	assert(initial_max > 0);
	
	DArray *array = malloc(sizeof(DArray));
	check_mem(array);

	array->max = initial_max;
	check(array->max > 0, "You must set a initial_max > 0.");

	array->contents = calloc(initial_max, sizeof(void *));
	check_mem(array->contents);

	array->end = 0;
	array->element_size = element_size;
	array->expand_rate = DEFAULT_EXPAND_RATE;

	assert(array != NULL);
	return array;

error:
	if (array->contents) free(array->contents);
	if (array) free(array);
	return NULL;
}

void DArray_clear(DArray *array)
{
	assert(array != NULL);

	int i = 0;

	if (array->element_size > 0) {
		for (i = 0; i < array->max; i++) {
			if (array->contents[i] != NULL) {
				free(array->contents[i]);
			}
		}
	}
}

static inline int DArray_resize(DArray *array, size_t newsize)
{
	assert(array != NULL && newsize > 0);
	
	array->max = newsize;
	check(array->max > 0, "The newsize must be > 0.");

	void *contents = realloc(array->contents, array->max * sizeof(void *));
	/* check contents and assume realloc doesn't harm the original on error */

	check_mem(contents);

	array->contents = contents;
	return 0;

error:
	return -1;
}

int DArray_expand(DArray *array)
{
	assert(array != NULL);
	
	size_t old_max = array->max;
	check(DArray_resize(array, array->max + array->expand_rate) == 0, "Failed to expand array to new size: %d", array->max + (int)array->expand_rate);

	memset(array->contents + old_max, 0, array->expand_rate * sizeof(void *));
	return 0;

error:
	return -1;
}

int DArray_contract(DArray *array)
{
	assert(array != NULL);

	int new_size = array->end  < (int)array->expand_rate ? (int)array->expand_rate : array->end;

	return DArray_resize(array, new_size);
}

void DArray_destroy(DArray *array)
{
	if (array) {
		if (array->contents) free(array->contents);
		free(array);
	}
}

void DArray_clear_destroy(DArray *array)
{
	DArray_clear(array);
	DArray_destroy(array);
}

int DArray_push(DArray *array, void *el)
{
	assert(array != NULL && el != NULL);

	array->contents[array->end] = el;
	array->end++;

	if (DArray_end(array) >= DArray_max(array)) {
		return DArray_expand(array);
	} else {
		return 0;
	}
}

void *DArray_pop(DArray *array)
{
	assert(array != NULL && array->end > 1);

	check(array->end - 1 >= 0, "Attempt to pop from empty array.");

	void *el = DArray_remove(array, array->end - 1);
	array->end--;

	if (DArray_end(array) > (int)array->expand_rate && DArray_end(array) % array->expand_rate) {
		DArray_contract(array);
	}
	assert(el != NULL);
	return el;

error:
	return NULL;
}

