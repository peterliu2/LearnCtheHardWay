/**
 * @file tstree.h
 * @brief Ternary Search Tree header
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-25
 */

#ifndef _tstree_h_
#define _tstree_h_

#include <stdlib.h>
#include <lcthw/darray.h>

typedef struct TSTree {
	char splitchar;
	struct TSTree *low;
	struct TSTree *equal;
	struct TSTree *high;
	void *value;
} TSTree;

void *TSTree_search(TSTree *root, const char *key, size_t len);
void *TSTree_search_prefix(TSTree *root, const char *key, size_t len);

typedef void (*TSTree_traverse_cb)(void *value, void *data);

TSTree *TSTree_insert(TSTree *node, const char *key, size_t len, void *value);
void TSTree_traverse(TSTree *node, TSTree_traverse_cb cb, void *data);
void TSTree_destroy(TSTree *root);
#endif /* _tstree_h_ */
