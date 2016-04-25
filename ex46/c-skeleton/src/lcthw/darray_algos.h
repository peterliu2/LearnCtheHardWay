/**
 * @file darray_algos.h
 * @brief Dynamic array sorting and searching implement
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-19
 */

#ifndef _darray_algos_h_
#define _darray_algos_h_

#include <lcthw/darray.h>

typedef int (*DArray_compare)(const void *a, const void *b);

int DArray_qsort(DArray *array, DArray_compare cmp);
int DArray_heapsort(DArray *array, DArray_compare cmp);
int DArray_mergesort(DArray *array, DArray_compare cmp);

#endif /* _darray_algos_h_ */
