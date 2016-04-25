/**
 * @file radixmap.h
 * @brief Radix sort header
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-19
 */

#ifndef _radixmap_h_
#define _radixmap_h_

#include <stdint.h>

typedef union RMElement {
	uint64_t raw;
	struct {
		uint32_t key;
		uint32_t value;
	} data;
} RMElement;

typedef union UINT32 {
	uint32_t ui23;
	struct {
		uint8_t _0;
		uint8_t _1;
		uint8_t _2;
		uint8_t _3;
	} byte;

} UINT32_T;


typedef struct RadixMap {
	size_t max;
	size_t end;
	uint32_t counter;
	UINT32_T key_max;
	UINT32_T key_min;
	RMElement *contents;
	RMElement *temp;
} RadixMap;


RadixMap *RadixMap_create(size_t max);
void RadixMap_destroy(RadixMap *map);
void RadixMap_sort(RadixMap *map);
RMElement *RadixMap_find(RadixMap *map, uint32_t key);
int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value);
int RadixMap_delete(RadixMap *map, RMElement *el);

#endif /* _radixmap_h_ */
