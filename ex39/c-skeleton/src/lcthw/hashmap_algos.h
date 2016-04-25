/**
 * @file hashmap_algos.h
 * @brief hash map algorithm header
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-21
 */

#ifndef _hashmap_algos_h_
#define _hashmap_algos_h_

#include <stdint.h>

uint32_t Hashmap_jenkin_hash(void *key);
uint32_t Hashmap_fnv1a_hash(void *key);
uint32_t Hashmap_adler32_hash(void *key);
uint32_t Hashmap_djb_hash(void *key);

#endif /* _hashmap_algos_h_ */
