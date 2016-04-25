/**
 * @file bstree.h
 * @brief Binary Search Tree header
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-22
 */

#ifndef _BSTREE_H_
#define _BSTREE_H_

typedef int (*BSTree_compare)(void *a, void *b);

typedef struct BSTreeNode {
	void *key;
	void *data;

	struct BSTreeNode *left;
	struct BSTreeNode *right;
	struct BSTreeNode *parent;
} BSTreeNode;

typedef struct BSTree {
	int count;
	BSTree_compare compare;
	BSTreeNode *root;
} BSTree;


typedef int (*BSTree_traverse_cb) (BSTreeNode *node);

BSTree *BSTree_create(BSTree_compare compare);
void BSTree_destroy(BSTree *map);

int BSTree_set(BSTree *map, void *key, void *value);
void *BSTree_get(BSTree *map, void *key);

int BSTree_traverse(BSTree *map, BSTree_traverse_cb traverse_cd);

void *BSTree_delete(BSTree *map, void *key);

#endif /* _BSTREE_H_ */
