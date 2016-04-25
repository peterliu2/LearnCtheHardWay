/**
 * @file darray_algos.c
 * @brief Dynamic array sorting and searching implememt
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-19
 */

#include <lcthw/darray_algos.h>
#include <bsd/stdlib.h>

int DArray_qsort(DArray *array, DArray_compare cmp)
{
	qsort(array->contents, DArray_count(array), sizeof(void *), cmp);
	return 0;
}

int DArray_heapsort(DArray *array, DArray_compare cmp)
{
	return heapsort(array->contents, DArray_count(array), sizeof(void *) ,cmp);
}

int DArray_mergesort(DArray *array, DArray_compare cmp)
{
	return mergesort(array->contents, DArray_count(array), sizeof(void *) ,cmp);
}
