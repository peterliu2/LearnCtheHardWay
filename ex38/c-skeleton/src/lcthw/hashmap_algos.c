/**
 * @file hashmap_algos.c
 * @brief hash algorithm implment
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-21
 */

#include <lcthw/hashmap_algos.h>
#include <lcthw/bstrlib.h>

/* setting taken from  
 * http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
 */
const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;

uint32_t Hashmap_jenkin_hash(void *key)
{
	bstring s = (bstring)key;
	uint32_t hash = 0;
	uint32_t i = 0;

	for (hash = i = 0; i < blength(s); i++) {
		hash += bchare(s, i, 0);
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}



uint32_t Hashmap_fnv1a_hash(void *key)
{
	bstring s = (bstring)key;
	uint32_t hash = FNV_OFFSET_BASIS;
	int i = 0;

	for (i = 0; i < blength(s); i++) {
		hash ^= bchare(s, i, 0);
		hash *= FNV_PRIME;
	}

	return hash;
}

const int MOD_ADLER = 65521;

uint32_t Hashmap_adler32_hash(void *key)
{
	bstring s = (bstring)key;
	uint32_t a = 1, b = 0;
	int i = 0;

	for (i = 0; i < blength(s); i++) {
		a = (a + bchare(s, i, 0)) % MOD_ADLER;
		b = (b + a) % MOD_ADLER;
	}

	return (b << 16) | a;
}

uint32_t Hashmap_djb_hash(void *key)
{
	bstring s = (bstring)key;
	uint32_t hash = 5381;
	int i = 0;

	for (i = 0; i < blength(s); i++) {
		hash = ((hash << 5) + hash) + bchare(s, i, 0); /* hash * 33 + c */
	}

	return hash;
}
