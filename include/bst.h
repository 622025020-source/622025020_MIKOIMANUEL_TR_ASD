#ifndef BST_H
#define BST_H

#include "route.h"

/* Binary Search Tree berkunci ID rute. Node hanya MENUNJUK Route
 * (kepemilikan Route tetap di RouteList). */
typedef struct BstNode {
    Route *route;
    struct BstNode *left;
    struct BstNode *right;
} BstNode;

BstNode *bst_insert(BstNode *root, Route *route);
Route   *bst_search(BstNode *root, int id);
BstNode *bst_delete(BstNode *root, int id);
void     bst_preorder(const BstNode *root);
void     bst_inorder(const BstNode *root);
void     bst_postorder(const BstNode *root);
int      bst_height(const BstNode *root);
void     bst_free(BstNode *root);

#endif
