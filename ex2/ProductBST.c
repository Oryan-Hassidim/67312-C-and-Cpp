#ifdef OH_TESTS
#include "oh_tests/oh_tests_helpers.h"
#define malloc test_malloc
#define calloc test_calloc
#define free test_free
#define stderr get_test_stderr()
#endif // OH_TESTS

#include <string.h>
#include "ProductBST.h"

#define RIGHT 1
#define LEFT -1
#define CURRENT 0
#define PARSE_SUCCEED 1
#define PARSE_FAILED 0
#define DELIMITER ':'
#define SPACE ' '

// #############################################
//                 Helpers
// #############################################

void print_error(char error_message[])
{
	fprintf(stderr, ERROR);
	fprintf(stderr, error_message);
}

/**
* This function takes 2 strings and compares them using strcmp.
* @param s1 : first string
* @param s2 : second string;
* @return 0 if equals, -1 if the first is less then the second, -1 otherwise.
* **/
int get_order(Node* node, char* s2)
{
	int order = strcmp(node->product.name, s2);
	if (order == 0)
	{
		return CURRENT;
	}
	if (order < 0)
	{
		return RIGHT;
	}
	return LEFT;
}

int is_digit(char c)
{
	return '0' <= c && c <= '9';
}

int get_number_of_sons(Node* node)
{
	return (node->left_child != NULL) + (node->right_child != NULL);
}

int is_a_leaf(Node* node)
{
	return get_number_of_sons(node) == 0;
}

Node* get_son_by_name(Node* node, char* name)
{
	switch (get_order(node, name))
	{
	case CURRENT:
		return node;
	case LEFT:
		return node->left_child;
	case RIGHT:
		return node->right_child;
	default:
		return NULL;
	}
}

// #############################################
//              Memory Helpers
// #############################################

void free_node_with_product(Node* node)
{
	if (node == NULL)
	{
		return;
	}
	free(node->product.name);
	free(node);
}

char* allocate_and_cpy(const char* src)
{
	char* dest = malloc(strlen(src) + 1);
	if (!dest)
	{
		return NULL;
	}
	strcpy(dest, src);
	return dest;
}

// #############################################
//                add_product
// #############################################

/**
* This function allocates memory and crates new node
* with the given product name.
* @return pointer to the node if success, otherwise - NULL.
**/
Node* create_node(char* name, int quantity)
{
	Node* node = calloc(1, sizeof(Node));
	if (node == NULL)
	{
		print_error(ALLOCATION_FAILED);
		return NULL;
	}
	char* new_name = allocate_and_cpy(name);
	if (new_name == NULL)
	{
		print_error(ALLOCATION_FAILED);
		free(node);
		return NULL;
	}
	node->product.name = new_name;
	node->product.quantity = quantity;
	return node;
}

Node* add_product_core(Node* node, char* name, int quantity)
{
	if (node == NULL)
	{
		return create_node(name, quantity);
	}
	Node** son = NULL;
	switch (get_order(node, name))
	{
	case CURRENT:
		print_error(PRODUCT_EXISTS);
		return node;
	case LEFT:
		son = &(node->left_child);
		break;
	case RIGHT:
		son = &(node->right_child);
		break;
	default:
		return NULL;
	}
	Node* new_node = add_product_core(*son, name, quantity);
	if (new_node == NULL)
	{
		return NULL;
	}
	*son = new_node;
	return node;

}

Node* add_product(Node* root, char* name, int quantity)
{
	if (quantity <= 0)
	{
		print_error(INVALID_QUANTITY);
		return root;
	}
	if (name == NULL)
	{
		print_error(INVALID_POINTER);
		return root;
	}

	Node* new_node = add_product_core(root, name, quantity);
	if (new_node == NULL)
	{
		return NULL;
	}
	if (root == NULL)
	{
		return new_node;
	}
	return root;
}

// #############################################
//                 bulid_bst
// #############################################

/**
* the function takes a buffer in form <product name>: <quantity>,
* sets buffer to the <product name> and updates the quantity.
* @param buffer: the original line, and the name after the all to the function.
* @param quantity: pointer to set the quantity.
* @return PARSE_SUCCEED on success, PARSE_FAILED otherwise.
**/
int parse_line(char* buffer, int* quantity)
{
	// I didn't use sscanf because I wanted to use the original
	// buffer, without copy the name.

	//parse name
	int i = 0;
	for (; buffer[i] != DELIMITER || buffer[i+1] != SPACE; i++) {}

	buffer[i++] = '\0';
	// now buffer point to the name of the product
	i++;

	// parse quantity
	buffer += i;
	for (i = 0; is_digit(buffer[i]) || (i == 0 && buffer[i] == '-'); i++) {}

	if (buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\0')
	{
		return PARSE_FAILED;
	}
	buffer[i] = '\0';
	*quantity = strtol(buffer, NULL, 10);

	return PARSE_SUCCEED;
}

Node* build_bst(const char* filename)
{
	// validation
	if (filename == NULL)
	{
		print_error(FILE_OPEN_FAILED);
		return NULL;
	}
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		print_error(FILE_OPEN_FAILED);
		return NULL;
	}

	char buffer[MAX_LINE_LENGTH + 1] = { 0 };
	// name placeholder after parsing is buffer
	char* name = buffer;
	int quantity = 0;

	Node* root = NULL, * updated = NULL;
	while (fgets(buffer, MAX_LINE_LENGTH + 1, file))
	{
		if (parse_line(buffer, &quantity) == PARSE_FAILED)
		{
			print_error(INVALID_LINE);
			continue;
		}
		updated = add_product(root, name, quantity);
		if (updated == NULL)
		{
			if (root != NULL)
			{
				delete_tree(root);
			}
			fclose(file);
			return NULL;
		}
		root = updated;
	}
	fclose(file);
	return root;
}

// #############################################
//               delete_product
// #############################################

/**
* takes node with 2 children and updates it without free the
* memory of the node. returns the new node after the "deletion".
**/
Node* delete_node_with_two_sons_helper(Node* node)
{
	if (node->right_child->left_child == NULL)
	{
		node->right_child->left_child = node->left_child;
		return node->right_child;
	}

	Node* father = node->right_child;
	Node* son = node->right_child->left_child;
	while (son->left_child != NULL)
	{
		father = son;
		son = son->left_child;
	}

	father->left_child = son->right_child;
	son->left_child = node->left_child;
	son->right_child = node->right_child;
	return son;
}

Node* delete_node(Node* node)
{
	Node* ret = NULL; // the node 
	switch (get_number_of_sons(node))
	{
	case 1:
		if (node->left_child != NULL)
		{
			ret = node->left_child;
		}
		else
		{
			ret = node->right_child;
		}
		break;
	case 2:
		ret = delete_node_with_two_sons_helper(node);
		break;
	}
	free_node_with_product(node);
	return ret;
}

Node* delete_product_core(Node* root, char* name)
{
	if (root == NULL)
	{
		print_error(PRODUCT_NOT_FOUND);
		return NULL;
	}

	// pointer to the son that need to be updated.
	Node** son;
	switch (get_order(root, name))
	{
	case CURRENT:
		return delete_node(root);
	case LEFT:
		son = &(root->left_child);
		break;
	case RIGHT:
		son = &(root->right_child);
		break;
	default:
		return NULL;
	}
	Node* updated = delete_product_core(*son, name);
	*son = updated;
	return root;
}

Node* delete_product(Node* root, char* name)
{
	if (name == NULL)
	{
		print_error(INVALID_POINTER);
		return root;
	}
	return delete_product_core(root, name);
}

// #############################################
//                    else
// #############################################

Product* search_product(Node* root, char* name)
{
	if (name == NULL)
	{
		print_error(INVALID_POINTER);
		return NULL;
	}
	if (root == NULL)
	{
		// not found.
		return NULL;
	}

	switch (get_order(root, name))
	{
	case CURRENT:
		return &(root->product);
	case LEFT:
		return search_product(root->left_child, name);
	case RIGHT:
		return search_product(root->right_child, name);
	default:
		return NULL;
	}
}

void delete_tree(Node* root)
{
	if (root == NULL)
	{
		return;
	}
	delete_tree(root->left_child);
	delete_tree(root->right_child);
	free_node_with_product(root);
}

Node* update_quantity(Node* root, char* name, int amount_to_update)
{
	if (root == NULL || name == NULL)
	{
		print_error(INVALID_POINTER);
		return root;
	}

	Product* product = search_product(root, name);
	if (product == NULL)
	{
		print_error(PRODUCT_NOT_FOUND);
		return root;
	}
	product->quantity += amount_to_update;
	if (product->quantity < 0)
	{
		print_error(INVALID_QUANTITY);
		product->quantity -= amount_to_update;
	}
	else if (product->quantity == 0)
	{
		return delete_product(root, name);
	}

	return root;
}
