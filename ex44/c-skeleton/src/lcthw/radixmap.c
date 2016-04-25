/**
 * @file radixmap.c
 * @brief Radix sort implement
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-19
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lcthw/radixmap.h>
#include <lcthw/dbg.h>

RadixMap *RadixMap_create(size_t max)
{
	RadixMap *map = calloc(1, sizeof(RadixMap));
	check_mem(map);

	map->contents = calloc(max + 1, sizeof(RMElement));
	check_mem(map->contents);

	map->temp = calloc(max + 1, sizeof(RMElement));
	check_mem(map->temp);

	map->max = max;
	map->end = 0;

	map->key_max.ui23 = 0;
	map->key_min.ui23 = UINT32_MAX;
	return map;

error:
	if (map->temp) free(map->temp);
	if (map->contents) free(map->contents);
	if (map) free(map);
	return NULL;
}

void RadixMap_destroy(RadixMap *map)
{
	if (map) {
		free(map->contents);
		free(map->temp);
		free(map);
	}
}

#define ByteOf(x,y) (((uint8_t *)x)[(y)])

static inline void radix_sort(short offset, uint64_t max, uint64_t *source, uint64_t *dest)
{
	uint64_t count[256] = {0};
	uint64_t *cp = NULL;
	uint64_t *sp = NULL;
	uint64_t *end = NULL;
	uint64_t s = 0;
	uint64_t c = 0;

	/* count occurences of every byte value */
	for (sp = source, end = source + max; sp < end; sp++) {
		count[ByteOf(sp, offset)]++;
	}

	/* transform count into index by summing elements and storing into the same array */
	for (s = 0, cp = count, end = count + 256; cp < end; cp++) {
		c = *cp;
		*cp = s;
		s += c;
	}

	/* fill dest with the right values in the right place */
	for (sp = source, end = source + max; sp < end; sp++) {
		cp = count + ByteOf(sp, offset);
		dest[*cp] = *sp;
		++(*cp);
	}
}

static void RadixMap_sort_with_key(RadixMap *map, UINT32_T key) 
{
	uint64_t *source = &map->contents[0].raw;
	uint64_t *temp = &map->temp[0].raw;
	
	if (key.byte._3) {
		radix_sort(3, map->end, temp, source);
	} else if (key.byte._2) {
		radix_sort(2, map->end, source, temp);
		radix_sort(3, map->end, temp, source);
	} else if (key.byte._1) {
		radix_sort(1, map->end, temp, source);
		radix_sort(2, map->end, source, temp);
		radix_sort(3, map->end, temp, source);
	} else {
		radix_sort(0, map->end, source, temp);
		radix_sort(1, map->end, temp, source);
		radix_sort(2, map->end, source, temp);
		radix_sort(3, map->end, temp, source);
	}
}

void RadixMap_sort(RadixMap *map)
{
	uint64_t *source = &map->contents[0].raw;
	uint64_t *temp = &map->temp[0].raw;

	radix_sort(0, map->end, source, temp);
	radix_sort(1, map->end, temp, source);
	radix_sort(2, map->end, source, temp);
	radix_sort(3, map->end, temp, source);
}

RMElement *RadixMap_find(RadixMap *map, uint32_t to_find)
{
	int low = 0;
	int high = map->end - 1;
	RMElement *data = map->contents;

	while (low <= high) {
		int middle = low + (high - low) / 2;
		uint32_t key = data[middle].data.key;

		if (to_find < key) {
			high = middle - 1;
		} else if (to_find > key) {
			low = middle + 1;
		} else {
			return &data[middle];
		}
	}
	return NULL;
}

int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value) 
{
	check(key < UINT32_MAX, "Key can't be equal to UINT32_MAX.");

	RMElement element = {.data = {.key = key, .value = value}};
	check(map->end + 1 < map->max, "RadixMap is full.");

	map->contents[map->end++] = element;

	if(map->key_max.ui23 < key) map->key_max.ui23 = key;
	if(map->key_min.ui23 > key) map->key_min.ui23 = key;

	RadixMap_sort_with_key(map, (UINT32_T)key);

	return 0;

error:
	return -1;
}

int RadixMap_delete(RadixMap *map, RMElement *el)
{
	check(map->end > 0, "There is nothing to delete.");
	check(el != NULL, "Can't delete a NULL element.");

	uint32_t old_key = el->data.key;
	
	if (old_key == map->key_max.ui23) 
		map->key_max.ui23 = UINT32_MAX;
	
	if (old_key == map->key_min.ui23) {
		if (map->end > 1) {
			map->key_min.ui23 = map->contents[1].data.key;
		} else {
			map->key_min.ui23 = UINT32_MAX;
		}
	}
	el->data.key = UINT32_MAX;

	if (map->end > 1) {
		/* don't bother resorting a map of 1 length */
		RadixMap_sort_with_key(map, (UINT32_T)old_key);
	}

	map->end--;

	return 0;

error:
	return -1;
}
