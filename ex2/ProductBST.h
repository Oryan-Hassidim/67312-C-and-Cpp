//
// Created by Miryam on 14/10/2022.
//

#include <stdlib.h>
#include <stdio.h>

#ifndef _ARRAYBST_H_
#define _ARRAYBST_H_

#endif //_ARRAYBST_H_


#define MAX_LINE_LENGTH 1025

#define ERROR "ERROR: "
#define INVALID_POINTER "NULL pointer passed as an argument to a function.\n"
#define FILE_OPEN_FAILED "Failed to open file.\n"
#define ALLOCATION_FAILED "Failed to allocate memory.\n"
#define PRODUCT_EXISTS "Product name already exists.\n"
#define INVALID_QUANTITY "Quantity should be greater than zero.\n"
#define INVALID_LINE "Line should be in format: <product_name>: <quantity>.\n"
#define PRODUCT_NOT_FOUND "Product name was not found.\n"


typedef struct Product {
    char *name;
    int quantity;
} Product;

typedef struct Node {
    Product product;
    struct Node *right_child;
    struct Node *left_child;
} Node;


/**
 * This function adds product to the BST with a lexicographic order of the
 * Products names.
 * It is the responsibility of this function to allocate memory for new node
 * and for name's string.
 * You cannot assume that the name isn't already in the tree.
 * You cannot assume that the quantity is positive.
 * Hint - Consider using a recursive helper function.
 * @param root : pointer to the root node of the BST.
 * @param name : string containing product's name
 * @param quantity : integer representing the existing quantity from
 * this product.
 * @return pointer to the root node of BST after adding a new product.
 * In case of memory allocation failure - return NULL.
 * In case of invalid quantity (zero or negative) - print error massage to
 * stderr and return the original tree, means - without adding the product.
 */
Node *add_product (Node *root, char *name, int quantity);


/**
 * This function reads line after line from the file and
 * builds binary search tree. Each node at the BST represents different
 * product and contains its quantity. The order relation between the products
 * is according to a lexicographic order of the products names.
 * Hint - consider using the function add_product as a helper function.
 * @param filename : name of text file that contains names of products and
 * theirs quantities.
 * @return pointer to the root node of the BST, or null in case of memory
 * allocation failure.
 */
Node *build_bst (const char *filename);


/**
 * Given a binary search tree (root) and a name of product, this function
 * deletes the node with this name and returns root of the updated tree.
 * In case the name does not exists at the tree, the function will return the
 * root as is, and will print error massage to stderr.
 * Hint - consider the three cases of the node to be deleted:
 * 1) Node is a leaf (has no children): Simply remove the leaf from the tree.
 * 2) Node has only one child: The node of the child of the node to be deleted
 * is now father's child (more can be found at the PDF).
 * 3) Node has two children: Find inorder successor of the node. Copy
 * contents of the inorder successor to the node and delete the inorder
 * successor.
 * (Don't forget freeing old memory and allocating a new one validly.)
 * @param root : pointer to root node
 * @param name : string with name of product to be deleted
 * @return pointer to the updated BST root after deleting.
 * In case that input root is null, the function will return null.
 */
Node *delete_product (Node *root, char *name);


/**
 * Search product at BST by product's name.
 * @param root : pointer to root node of BST.
 * @param name : string with name to be searched.
 * @return pointer to the found product. In case the product does not exist,
 * or in case that the input root in NULL, return null.
 */
Product *search_product (Node *root, char *name);


/**
 * This function frees BST's allocated memory.
 * @param root : pointer to root node of BST.
 */
void delete_tree (Node *root);


/**
 * Update quantity of a product in the BST. You cannot assume the name is
 * already exists in the tree.
 * @param root : pointer to the root node of tree.
 * @param name : string with name of product to be updated.
 * @param amount_to_update : integer with amount to be updated. You should
 * add this value to the quantity of the product. If this value is negative,
 * You should subtract the absolute value from the existing quantity.
 * @return root of tree after performing the update.
 * In case of failure (name is not found, root is NULL or the **updated**
 * quantity is negative) - print error massage and do nothing (return
 * the original root).
 * In case that the **updated** quantity is exactly zero - return the root
 * of tree after deleting this product.
 */
Node* update_quantity (Node *root, char *name, int amount_to_update);
