#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

BstNode *bst_insert(BstNode *root, Route *route)
{
    if (root == NULL) {
        BstNode *node = (BstNode *)malloc(sizeof(BstNode));
        if (node == NULL) {
            return NULL;
        }
        node->route = route;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    if (route->id < root->route->id) {
        root->left = bst_insert(root->left, route);
    } else if (route->id > root->route->id) {
        root->right = bst_insert(root->right, route);
    }
    return root;
}

Route *bst_search(BstNode *root, int id)
{
    if (root == NULL) {
        return NULL;
    }
    if (id == root->route->id) {
        return root->route;
    }
    if (id < root->route->id) {
        return bst_search(root->left, id);
    }
    return bst_search(root->right, id);
}

static BstNode *bst_min_node(BstNode *node)
{
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }
    return node;
}

BstNode *bst_delete(BstNode *root, int id)
{
    BstNode *temp;

    if (root == NULL) {
        return NULL;
    }
    if (id < root->route->id) {
        root->left = bst_delete(root->left, id);
    } else if (id > root->route->id) {
        root->right = bst_delete(root->right, id);
    } else {
        if (root->left == NULL) {
            temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            temp = root->left;
            free(root);
            return temp;
        }
        temp = bst_min_node(root->right);
        root->route = temp->route;
        root->right = bst_delete(root->right, temp->route->id);
    }
    return root;
}

void bst_preorder(const BstNode *root)
{
    if (root == NULL) {
        return;
    }
    route_print_row(root->route);
    bst_preorder(root->left);
    bst_preorder(root->right);
}

void bst_inorder(const BstNode *root)
{
    if (root == NULL) {
        return;
    }
    bst_inorder(root->left);
    route_print_row(root->route);
    bst_inorder(root->right);
}

void bst_postorder(const BstNode *root)
{
    if (root == NULL) {
        return;
    }
    bst_postorder(root->left);
    bst_postorder(root->right);
    route_print_row(root->route);
}

int bst_height(const BstNode *root)
{
    int lh, rh;

    if (root == NULL) {
        return 0;
    }
    lh = bst_height(root->left);
    rh = bst_height(root->right);
    return (lh > rh ? lh : rh) + 1;
}

void bst_free(BstNode *root)
{
    if (root == NULL) {
        return;
    }
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}
