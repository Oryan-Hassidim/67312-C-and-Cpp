#ifdef _WIN32
#include "oh_tests/oh_tests_helpers.h"
#else // linux
#include "/cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/oh_tests/oh_tests_helpers.h"
#endif //_WIN32
#include "markov_chain.h"
#include <string.h>

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define MAX_GENERATION_LENGTH 60

#define ALLOWED_ARGC 3
#define NEW_LINE "\n"

#define ARGUMENT_COUNT_ERROR_MESSAGE \
  "USAGE: snakes_and_ladders <seed>" \
  " <routes count>\n"

typedef struct Cell* cell;

int END;

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell
{
	int* a;
	int* b;
} Cell;

// I very very hate *, so I using lowercase for pointers.
// for call to the struct itself, use "struct" prefix and uppercase.
typedef struct Node* node;
typedef struct LinkedList* linkedList;
typedef void* T;
typedef struct NextNodeCounter* nextNodeCounterArray;
typedef struct MarkovNode* markovNode;
typedef struct MarkovChain* markovChain;

// ####################################################
//                  string helpers
// ####################################################

typedef char* str;

static int str_to_int(str s)
{
	return strtol(s, NULL, 10);
}

// ####################################################
//                   cell helpers
// ####################################################

cell cellize(T c)
{
	return c;
}

static void free_if_not_null(void* block)
{
	if (block != NULL)
	{
		free(block);
	}
}

static bool cell_is_last(const T _cell)
{
	cell c = _cell;
	return *c->a % END == 0;
}

static void cell_print(T _cell)
{
	cell c = (cell)_cell;
	if (cell_is_last(c))
	{
		printf("{ %d, %d } end", *c->a, *c->b);
		return;
	}
	printf("{ %d, %d } ", *c->a, *c->b);
}

static int cell_compare(const T c1, const T c2)
{
	int d = *((cell)c1)->a - *((cell)c2)->a;
	return d; // != 0 ? d : *((cell)c1)->b - *((cell)c2)->b;
}

static void cell_free(const T _cell)
{
	cell c = _cell;
	free(c->a);
	free(c->b);
	free(_cell);
}

static T cell_copy(const T _cell)
{
	cell new_cell = calloc(1, sizeof(struct Cell));
	int* a = calloc(1, sizeof(int));
	int* b = calloc(1, sizeof(int));
	if (new_cell == NULL || a == NULL || b == NULL)
	{
		free_if_not_null(new_cell);
		free_if_not_null(a);
		free_if_not_null(b);
		return NULL;
	}
	cell c = _cell;
	*a = *c->a;
	*b = *c->b;
	new_cell->a = a;
	new_cell->b = b;

	return new_cell;
}

// ####################################################
//                   flow helpers
// ####################################################

static int randomize()
{
	return rand() % (5 * END) + 1;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(markovChain markov_chain)
{
	int a = 0;
	int b = 0;
	Cell cell_data = { &a, &b };
	cell cell = &cell_data;

	markovNode from_node = NULL, to_node = NULL;

	for (int i = 0; i < END * 20; i++)
	{
		a = randomize();
		b = randomize();
		node node = add_to_database(markov_chain, cell);
		if (node == NULL)
			return EXIT_FAILURE;
		to_node = node->data;
		if (from_node != NULL && !add_node_to_counter_list(from_node, to_node, markov_chain))
			return EXIT_FAILURE;
		from_node = to_node;
	}
	a = END;
	b = END;
	node node = add_to_database(markov_chain, cell);
	if (node == NULL)
		return EXIT_FAILURE;
	to_node = node->data;
	if (from_node != NULL && !add_node_to_counter_list(from_node, to_node, markov_chain))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}


/**
 * creates and fills new markov chain
 * @param
 * @return pointer to a MarcovChain on success, NULL if allocation
 * error occurred.
 */
static markovChain load()
{
	markovChain markov_chain = calloc(1, sizeof(struct MarkovChain));
	linkedList database = calloc(1, sizeof(struct LinkedList));
	if (markov_chain == NULL || database == NULL)
	{
		free_if_not_null(markov_chain);
		free_if_not_null(database);
		return NULL;
	}
	markov_chain->database = database;
	markov_chain->comp_func = cell_compare;
	markov_chain->copy_func = cell_copy;
	markov_chain->free_data = cell_free;
	markov_chain->print_func = cell_print;
	markov_chain->is_last = cell_is_last;

	if (fill_database(markov_chain) == EXIT_FAILURE)
	{
		free_markov_chain(&markov_chain);
		return NULL;
	}
	return markov_chain;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char* argv[])
{
	if (argc > 3)
	{
		set_limit_allocs(str_to_int(argv[3]));
		argc--;
	}

	if (argc != ALLOWED_ARGC)
	{
		printf(ARGUMENT_COUNT_ERROR_MESSAGE);
		return EXIT_FAILURE;
	}

	set_allocations(0);

	END = rand() % 50 + 1;
	printf("END: % d\n\n", END);

	srand(str_to_int(argv[1]));

	markovChain markov_chain = load();
	if (markov_chain == NULL)
	{
		printf(ALLOCATION_ERROR_MASSAGE);
		// printf("\n\nAllocations that didn't free: %llu\n", get_allocations());
		return EXIT_FAILURE;
	}

	int routes = str_to_int(argv[2]);
	for (int i = 0; i < routes; i++)
	{
		printf("Random seq %d: ", i + 1);
		generate_random_sequence(markov_chain, NULL, MAX_GENERATION_LENGTH);
		printf(NEW_LINE);
	}

	free_markov_chain(&markov_chain);

	printf("\nmarkov_chain=%p\n", markov_chain);

	// printf("\n\nAllocations that didn't free: %llu\n", get_allocations());
	return EXIT_SUCCESS;
}
