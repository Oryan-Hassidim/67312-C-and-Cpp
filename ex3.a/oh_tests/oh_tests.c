// C & C++ Course 67312
// ex2 Tests
// Oryan Hassidim
// Oryan.Hassidim@mail.huji.ac.il


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../ProductBST.h"
#include "oh_tests_helpers.h"

#define SPACES "        "
#define SUCCESS 1
#define FAILURE 0

#define print_red(format, ...) printf("\033[0;31m" format "\033[0m", ##__VA_ARGS__)
#define print_green(format, ...) printf("\033[0;32m" format "\033[0m", ##__VA_ARGS__)

#pragma region Helpers
typedef struct Strings {
	int size;
	char** strings;
} Strings;

char stderr_filename[50] = { 0 };

int open_write_stderr()
{
	sprintf(stderr_filename, "stderr%d", rand());
	FILE* test_stderr = fopen(stderr_filename, "w");

	if (test_stderr == NULL) return 0;
	set_test_stderr(test_stderr);
	return 1;
}

int close_stderr()
{
	return fclose(get_test_stderr()) == 0;
}

int str_equals(char* s1, char* s2)
{
	return strcmp(s1, s2) == 0;
}

int print_whole_file(char* file_name)
{
	FILE* f = fopen(file_name, "r");
	if (f == NULL) return 0;
	char buffer[512] = { 0 };
	while (fgets(buffer, 511, f))
	{
		printf(buffer);
	}
	fclose(f);
}

int starts_with_error(char* line)
{
	char temp = line[7];
	line[7] = '\0';
	int answer = str_equals(line, ERROR);
	line[7] = temp;
	return answer;
}

int compare_stderr_to_expected(Strings* strings)
{
	int ans = 1;
	FILE* stderr_file = fopen(stderr_filename, "r");
	if (stderr_file == NULL) {
		print_red("ERROR: Could not open stderr file.\n");
		return FAILURE;
	}

	int i = 0;
	char line[100];
	while (fgets(line, 100, stderr_file) != NULL) {
		if (i >= strings->size) {
			ans = 0;
			break;
		}
		if (!starts_with_error(line) && !str_equals(line + 7, strings->strings[i])) {
			ans = 0;
			break;
		}
		i++;
	}

	if (ans && i < strings->size) {
		print_red("ERROR: Too few lines in stderr.\n");
		ans = 0;
	}

	fclose(stderr_file);

	if (!ans)
	{
		print_red("stderr is diferrent from expected!\n\n");
		printf("Expected:\n");
		for (size_t i = 0; i < strings->size; i++)
		{
			printf(ERROR);
			printf(strings->strings[i]);
		}
		printf("\nGot:\n");
		print_whole_file(stderr_filename);
		printf("\n");
	}

	remove(stderr_filename);
	return ans;
}

int compare_output_to_expected(char* output_file_name, char* expected_file_name)
{
	FILE* f1 = fopen(output_file_name, "r"), * f2 = fopen(expected_file_name, "r");
	if (f1 == NULL || f2 == NULL)
	{
		if (f1 != NULL) fclose(f1);
		if (f2 != NULL) fclose(f2);
		return 0;
	}
	char buffer1[512] = { 0 }, buffer2[512] = { 0 };
	char* b1 = fgets(buffer1, 511, f1), * b2 = fgets(buffer2, 511, f2);
	while (b1 && b2)
	{
		if (!str_equals(buffer1, buffer2))
		{
			fclose(f1);
			fclose(f2);
			return 0;
		}
		b1 = fgets(buffer1, 511, f1);
		b2 = fgets(buffer2, 511, f2);
	}
	int answer = 1;
	if (feof(f1) != feof(f2)) answer = 0;
	fclose(f1);
	fclose(f2);
	return answer;
}

void print_tree_core(Node* root, FILE* stream, int level)
{
	if (root == NULL)
	{
		return;
	}
	print_tree_core(root->left_child, stream, level + 1);
	for (int i = 0; i < level; i++)
	{
		fputs(SPACES, stream);
	}
	fprintf(stream, "{ %s: %d }\n", root->product.name, root->product.quantity);
	print_tree_core(root->right_child, stream, level + 1);
}

int print_tree(Node* root, char* file_name)
{
	FILE* f = fopen(file_name, "w");
	if (f == NULL)
	{
		return EXIT_SUCCESS;
	}
	print_tree_core(root, f, 0);
	fclose(f);
}
#pragma endregion

#pragma region Tests
#pragma region add_product
// 1
int test_add_children()
{
	if (!open_write_stderr()) return FAILURE;

	Node n = { {"Dell XPS 13", 500}, NULL, NULL };
	Node* node = &n;
	node = add_product(node, "Acer 15", 96);
	node = add_product(node, "Lenovo ThinkPad 13", 12);

	int answer =
		node->left_child->product.quantity == 96
		&& node->right_child->product.quantity == 12
		&& str_equals(node->left_child->product.name, "Acer 15")
		&& str_equals(node->right_child->product.name, "Lenovo ThinkPad 13");
	delete_tree(node->left_child);
	delete_tree(node->right_child);

	node->left_child = NULL;
	node->right_child = NULL;

	node = add_product(node, "Lenovo ThinkPad 13", 12);
	node = add_product(node, "Acer 15", 96);

	answer = answer &&
		node->left_child->product.quantity == 96
		&& node->right_child->product.quantity == 12
		&& str_equals(node->left_child->product.name, "Acer 15")
		&& str_equals(node->right_child->product.name, "Lenovo ThinkPad 13");
	delete_tree(node->left_child);
	delete_tree(node->right_child);


	if (!close_stderr()) return FAILURE;
	Strings expected = { 0, NULL };
	return answer && compare_stderr_to_expected(&expected);
}
// 2
int test_add_to_null()
{
	if (!open_write_stderr()) return FAILURE;
	Node* node = add_product(NULL, "Dell XPS 13", 500);
	int answer =
		node != NULL
		&& node->left_child == NULL
		&& node->right_child == NULL
		&& str_equals(node->product.name, "Dell XPS 13")
		&& node->product.quantity == 500;
	delete_tree(node);
	if (!close_stderr()) return FAILURE;
	Strings expected = { 0, NULL };
	return answer && compare_stderr_to_expected(&expected);
}
// 3
int test_add_negative()
{
	if (!open_write_stderr()) return FAILURE;

	Node n = { {"Dell XPS 13", 500}, NULL, NULL };
	Node* node = &n;
	node = add_product(node, "Acer 15", -96);
	node = add_product(node, "Lenovo ThinkPad 13", 12);

	int answer =
		node != NULL
		&& node->left_child == NULL
		&& node->right_child->product.quantity == 12
		&& str_equals(node->right_child->product.name, "Lenovo ThinkPad 13");
	delete_tree(node->right_child);

	if (!close_stderr()) return FAILURE;
	char* errors[1] = { INVALID_QUANTITY };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}
// 4
int test_add_zero()
{
	if (!open_write_stderr()) return FAILURE;

	Node n = { {"Dell XPS 13", 500}, NULL, NULL };
	Node* node = &n;
	node = add_product(node, "Acer 15", 0);
	node = add_product(node, "Lenovo ThinkPad 13", 12);

	int answer =
		node != NULL
		&& node->left_child == NULL
		&& node->right_child->product.quantity == 12
		&& str_equals(node->right_child->product.name, "Lenovo ThinkPad 13");
	delete_tree(node->right_child);

	if (!close_stderr()) return FAILURE;
	char* errors[1] = { INVALID_QUANTITY };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}
// 5
int test_add_name_null()
{
	if (!open_write_stderr()) return FAILURE;

	Node n = { {"Dell XPS 13", 500}, NULL, NULL };
	Node* node = &n;
	node = add_product(node, NULL, 15);
	node = add_product(node, "Lenovo ThinkPad 13", 12);

	int answer =
		node != NULL
		&& node->left_child == NULL
		&& node->right_child->product.quantity == 12
		&& str_equals(node->right_child->product.name, "Lenovo ThinkPad 13");
	delete_tree(node->right_child);

	if (!close_stderr()) return FAILURE;
	char* errors[1] = { INVALID_POINTER };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}
// 6
int test_add_exists_product()
{
	if (!open_write_stderr()) return FAILURE;

	Node n = { {"Dell XPS 13", 500}, NULL, NULL };
	Node* node = &n;
	node = add_product(node, "Lenovo ThinkPad 13", 15);
	node = add_product(node, "Lenovo ThinkPad 13", 12);

	int answer =
		node != NULL
		&& node->left_child == NULL
		&& node->right_child->product.quantity == 15
		&& str_equals(node->right_child->product.name, "Lenovo ThinkPad 13");
	delete_tree(node->right_child);

	if (!close_stderr()) return FAILURE;
	char* errors[1] = { PRODUCT_EXISTS };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}
#pragma endregion

#pragma region build_bst
// 7
int test_build_tree()
{
	if (!open_write_stderr()) return FAILURE;

	char* outputfile = "test_build_tree_output.txt";

	Node* root = build_bst("products_s.txt");
	print_tree(root, outputfile);
	delete_tree(root);
	int answer = compare_output_to_expected(outputfile, "products_s_ex.txt");

	if (!answer)
	{
		print_red("Error! your tree is different from expected!\n\n");
		printf("Expected:\n\n");
		print_whole_file("products_s_ex.txt");
		printf("\nGot:\n\n");
		print_whole_file(outputfile);
		printf("\n");
	}
	remove(outputfile);

	if (!close_stderr()) return FAILURE;
	Strings expected = { 0, NULL };
	return answer && compare_stderr_to_expected(&expected);
}
// 8
int test_build_tree_null_name()
{
	if (!open_write_stderr()) return FAILURE;

	Node* root = build_bst(NULL);

	int answer = root == NULL;

	if (!close_stderr()) return FAILURE;
	char* errors[1] = { FILE_OPEN_FAILED };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}
// 9
int test_build_tree_with_zero_or_negative()
{
	if (!open_write_stderr()) return FAILURE;

	char* outputfile = "test_build_tree_with_zero_or_negative_output.txt";

	Node* root = build_bst("products_s_zn.txt");
	print_tree(root, outputfile);
	delete_tree(root);
	int answer = compare_output_to_expected(outputfile, "products_s_zn_ex.txt");

	if (!answer)
	{
		print_red("Error! your tree is different from expected!\n\n");
		printf("Expected:\n\n");
		print_whole_file("products_s_zn_ex.txt");
		printf("\nGot:\n\n");
		print_whole_file(outputfile);
		printf("\n");
	}
	remove(outputfile);

	if (!close_stderr()) return FAILURE;
	char* errors[] = { INVALID_QUANTITY, INVALID_QUANTITY };
	Strings expected = { 2, errors };
	return answer && compare_stderr_to_expected(&expected);
}
// 10
int test_build_tree_with_invalid_lines()
{
	if (!open_write_stderr()) return FAILURE;

	char* outputfile = "test_build_tree_with_invalid_lines_output.txt";

	Node* root = build_bst("products_s_b.txt");
	print_tree(root, outputfile);
	delete_tree(root);
	int answer = compare_output_to_expected(outputfile, "products_s_zn_ex.txt");

	if (!answer)
	{
		print_red("Error! your tree is different from expected!\n\n");
		printf("Expected:\n\n");
		print_whole_file("products_s_zn_ex.txt");
		printf("\nGot:\n\n");
		print_whole_file(outputfile);
		printf("\n");
	}
	remove(outputfile);

	if (!close_stderr()) return FAILURE;
	char* errors[2] = { INVALID_LINE, INVALID_LINE };
	Strings expected = { 2, errors };
	return answer && compare_stderr_to_expected(&expected);
}

#pragma endregion

#pragma region delete_product
// 11
int test_delete_leaf()
{
	if (!open_write_stderr()) return FAILURE;


	Node* root = build_bst("products_s.txt");
	root = delete_product(root, "Dell XPS");
	int answer =
		root != NULL
		&& root->right_child->left_child->left_child->left_child->right_child == NULL
		&& str_equals(root->right_child->left_child->left_child->left_child->product.name, "Dell Vostro");

	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	Strings expected = { 0, NULL };
	return answer && compare_stderr_to_expected(&expected);
}
// 12
int test_delete_with_one_child()
{
	if (!open_write_stderr()) return FAILURE;


	Node* root = build_bst("products_s.txt");
	root = delete_product(root, "HP Elitebook");
	int answer =
		root != NULL
		&& root->right_child->left_child->left_child != NULL
		&& str_equals(root->right_child->left_child->left_child->product.name, "Dell Vostro");

	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	Strings expected = { 0, NULL };
	return answer && compare_stderr_to_expected(&expected);
}
// 13
int test_delete_with_two_children()
{
	if (!open_write_stderr()) return FAILURE;


	Node* root = build_bst("delete_ex.txt");
	root = delete_product(root, "f");
	int answer =
		root != NULL
		&& root->right_child != NULL
		&& str_equals(root->right_child->product.name, "g")
		&& root->right_child->product.quantity == 7
		&& str_equals(root->right_child->right_child->left_child->left_child->product.name, "i")
		&& str_equals(root->right_child->right_child->left_child->left_child->right_child->product.name, "j");

	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	Strings expected = { 0, NULL };
	return answer && compare_stderr_to_expected(&expected);
}
// 14
int test_delete_null_tree()
{
	if (!open_write_stderr()) return FAILURE;


	Node* node = delete_product(NULL, "Dell XPS");
	int answer = node == NULL;

	if (!close_stderr()) return FAILURE;
	char* errors[] = { PRODUCT_NOT_FOUND };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}
// 15
int test_delete_null_name()
{
	if (!open_write_stderr()) return FAILURE;


	Node* root = build_bst("products_s.txt");
	root = delete_product(root, NULL);
	int answer =
		root != NULL
		&& root->left_child->right_child->product.quantity == 64;

	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	char* errors[] = { INVALID_POINTER };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}
// 16
int test_delete_unfound_name()
{
	if (!open_write_stderr()) return FAILURE;


	Node* root = build_bst("products_s.txt");
	root = delete_product(root, "Dell");
	int answer =
		root != NULL
		&& root->right_child->left_child->left_child->left_child->left_child->left_child->product.quantity == 542;

	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	char* errors[] = { PRODUCT_NOT_FOUND };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}

#pragma endregion

#pragma region search_product
// 17
int test_search_product()
{
	if (!open_write_stderr()) return FAILURE;

	Node* root = build_bst("products_s.txt");
	Product* product = search_product(root, "Dell Vostro");
	int answer = str_equals(product->name, "Dell Vostro") && product->quantity == 448;
	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	Strings expected = { 0, NULL };
	return answer && compare_stderr_to_expected(&expected);
}
// 18
int test_search_product_unfound_name()
{
	if (!open_write_stderr()) return FAILURE;

	Node* root = build_bst("products_s.txt");
	Product* product = search_product(root, "Dell");
	int answer = product == NULL;
	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	Strings expected = { 0, NULL };
	return answer && compare_stderr_to_expected(&expected);
}
// 19
int test_search_product_null_root()
{
	if (!open_write_stderr()) return FAILURE;

	Node* root = build_bst("products_s.txt");
	Product* product = search_product(NULL, "Dell Vostro");
	int answer = product == NULL;
	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	Strings expected = { 0, NULL };
	return answer && compare_stderr_to_expected(&expected);
}
// 20
int test_search_product_null_name()
{
	if (!open_write_stderr()) return FAILURE;

	Node* root = build_bst("products_s.txt");
	Product* product = search_product(root, NULL);
	int answer = product == NULL;
	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	char* errors[] = { INVALID_POINTER };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}

#pragma endregion

#pragma region update_quantity
// 21
int test_update_quantity()
{
	if (!open_write_stderr()) return FAILURE;

	Node* root = build_bst("products_s.txt");
	root = update_quantity(root, "Dell Vostro", 100);
	int answer =
		root != NULL
		&& root->right_child->left_child->left_child->left_child->product.quantity == 548
		&& str_equals(root->right_child->left_child->left_child->left_child->product.name, "Dell Vostro");

	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	Strings expected = { 0, NULL };
	return answer && compare_stderr_to_expected(&expected);
}
// 22
int test_update_quantity_null_root()
{
	if (!open_write_stderr()) return FAILURE;

	int answer =
		update_quantity(NULL, "Dell Vostro", 100) == NULL;

	if (!close_stderr()) return FAILURE;
	char* errors[] = { INVALID_POINTER };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}
// 23
int test_update_quantity_null_name()
{
	if (!open_write_stderr()) return FAILURE;

	Node* root = build_bst("products_s.txt");
	root = update_quantity(root, NULL, 100);
	int answer =
		root != NULL;

	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	char* errors[] = { INVALID_POINTER };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}
// 24
int test_update_quantity_unfound_name()
{
	if (!open_write_stderr()) return FAILURE;

	Node* root = build_bst("products_s.txt");
	root = update_quantity(root, "Dell", 100);
	int answer =
		root != NULL
		&& root->right_child->left_child->left_child->left_child->product.quantity == 448;

	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	char* errors[] = { PRODUCT_NOT_FOUND };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}
// 25
int test_update_quantity_to_zero()
{
	if (!open_write_stderr()) return FAILURE;

	Node* root = build_bst("products_s.txt");
	root = update_quantity(root, "Dell Vostro", -448);
	int answer =
		root != NULL
		&& search_product(root, "Dell Vostro") == NULL;

	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	Strings expected = { 0, NULL };
	return answer && compare_stderr_to_expected(&expected);
}
// 26
int test_update_quantity_to_negative()
{
	if (!open_write_stderr()) return FAILURE;

	Node* root = build_bst("products_s.txt");
	root = update_quantity(root, "Dell Vostro", -449);
	int answer =
		root != NULL
		&& search_product(root, "Dell Vostro")->quantity == 448
		&& str_equals(root->right_child->left_child->left_child->left_child->product.name, "Dell Vostro");

	delete_tree(root);

	if (!close_stderr()) return FAILURE;
	char* errors[] = { INVALID_QUANTITY };
	Strings expected = { 1, errors };
	return answer && compare_stderr_to_expected(&expected);
}
#pragma endregion


#pragma endregion



int (*tests[26]) () =
{
	test_add_children,
	test_add_to_null,
	test_add_negative,
	test_add_zero,
	test_add_name_null,
	test_add_exists_product,
	test_build_tree,
	test_build_tree_null_name,
	test_build_tree_with_zero_or_negative,
	test_build_tree_with_invalid_lines,
	test_delete_leaf,
	test_delete_with_one_child,
	test_delete_with_two_children,
	test_delete_null_tree,
	test_delete_null_name,
	test_delete_unfound_name,
	test_search_product,
	test_search_product_unfound_name,
	test_search_product_null_root,
	test_search_product_null_name,
	test_update_quantity,
	test_update_quantity_null_root,
	test_update_quantity_null_name,
	test_update_quantity_unfound_name,
	test_update_quantity_to_zero,
	test_update_quantity_to_negative
};

int test(int test_number)
{
	set_allocations(0);

	int result = tests[test_number]();
	if (result)
	{
		print_green("Test %d succeed :)\n", test_number + 1);
	}
	else
	{
		print_red("Test %d failed :(\n", test_number + 1);
	}

	if (get_allocations() != 0)
	{
		print_red("Memory leak :(\n%d allocations didn't free in test number %d.\n\n",
			get_allocations(), test_number + 1);
		result = EXIT_FAILURE;
	}
	return result;
}

/**
 * main to run functions from Ex2
 */
int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		int succeed = 0, tests_number = sizeof(tests) / sizeof(tests[0]);
		for (size_t i = 0; i < tests_number; i++)
		{
			succeed += test(i) ? 1 : 0;
		}

		if (succeed == tests_number)
		{
			print_green(
				"\n*************************\n\n  All tests succeed :)\n\n*************************\n");
		}
		else
		{
			print_red(
				"\n*****************************\n\n%d out of %d tests failed :(\n\n*****************************\n",
				tests_number - succeed, tests_number);
		}
		return EXIT_SUCCESS;
	}
	int test_number = strtol(argv[1], NULL, 10);

	int result = test(test_number - 1);
	return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
