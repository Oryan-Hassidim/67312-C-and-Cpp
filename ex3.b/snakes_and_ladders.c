#ifdef OH_TESTS
#ifdef _WIN32
#include "oh_tests/oh_tests_helpers.h"
#else // linux
#include "/cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/oh_tests/oh_tests_helpers.h"
#endif //_WIN32
#endif // OH_TESTS

#include "markov_chain.h"
#include <string.h> // For strlen(), strcmp(), strcpy()

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

#define ALLOWED_ARGC 3
#define LADDER_CELL_TEMPLATE "[%d]-ladder to %d -> "
#define SNAKE_CELL_TEMPLATE "[%d]-snake to %d -> "
#define END_CELL_TEMPLATE "[%d]"
#define REGULAR_CELL_TEMPLATE "[%d] -> "
#define NEW_LINE "\n"

#define ARGUMENT_COUNT_ERROR_MESSAGE \
  "USAGE: snakes_and_ladders <seed>" \
  " <journies count>\n"

#define VALIDATE_ARGS(e, fallback_value) \
if (!e) \
{ \
	return fallback_value; \
}

typedef struct Cell* cell;

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2]
= { { 13, 4 },  { 85, 17 }, { 95, 67 }, { 97, 58 }, { 66, 89 },
	{ 87, 31 }, { 57, 83 }, { 91, 25 }, { 28, 50 }, { 35, 11 },
	{ 8, 30 },  { 41, 62 }, { 81, 43 }, { 69, 32 }, { 20, 39 },
	{ 33, 70 }, { 79, 99 }, { 23, 76 }, { 15, 47 }, { 61, 14 } };

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell
{
	int number;    // Cell number 1-100
	int ladder_to; // ladder_to represents the jump of the ladder in case there
	// is one from this square
	int snake_to;  // snake_to represents the jump of the snake in case there is
	// one from this square
// both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

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

static void cell_print(T _cell)
{
	cell c = (cell)_cell;
	if (c->ladder_to != EMPTY)
	{
		printf(LADDER_CELL_TEMPLATE, c->number, c->ladder_to);
	}
	else if (c->snake_to != EMPTY)
	{
		printf(SNAKE_CELL_TEMPLATE, c->number, c->snake_to);
	}
	else if (c->number == BOARD_SIZE)
	{
		printf(END_CELL_TEMPLATE, c->number);
	}
	else
	{
		printf(REGULAR_CELL_TEMPLATE, c->number);
	}
}

static bool cell_is_last(const T _cell)
{
	return ((cell)_cell)->number == BOARD_SIZE;
}

static int cell_compare(const T c1, const  T c2)
{
	return ((cell)c1)->number - ((cell)c2)->number;
}

static void cell_free(const T _cell)
{
	free(_cell);
}

static T cell_copy(const T _cell)
{
	cell new_cell = calloc(1, sizeof(struct Cell));
	if (new_cell == NULL)
	{
		return NULL;
	}
	memcpy(new_cell, _cell, sizeof(struct Cell));
	return new_cell;
}

// ####################################################
//                   flow helpers
// ####################################################

static int create_board(cell cells[BOARD_SIZE])
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		cells[i] = malloc(sizeof(Cell));
		if (cells[i] == NULL)
		{
			for (int j = 0; j < i; j++)
			{
				free(cells[j]);
			}
			return EXIT_FAILURE;
		}
		*(cells[i]) = (Cell){ i + 1, EMPTY, EMPTY };
	}

	for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
	{
		int from = transitions[i][0];
		int to = transitions[i][1];
		if (from < to)
		{
			cells[from - 1]->ladder_to = to;
		}
		else
		{
			cells[from - 1]->snake_to = to;
		}
	}
	return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(markovChain markov_chain)
{
	cell cells[BOARD_SIZE];
	if (create_board(cells) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
	markovNode from_node = NULL, to_node = NULL;
	size_t index_to;
	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		add_to_database(markov_chain, cells[i]);
	}

	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		from_node = get_node_from_database(markov_chain, cells[i])->data;

		if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
		{
			index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
			to_node
				= get_node_from_database(markov_chain, cells[index_to])->data;
			add_node_to_counter_list(from_node, to_node, markov_chain);
		}
		else
		{
			for (int j = 1; j <= DICE_MAX; j++)
			{
				index_to = ((cell)(from_node->data))->number + j - 1;
				if (index_to >= BOARD_SIZE)
				{
					break;
				}
				to_node = get_node_from_database(markov_chain, cells[index_to])
					->data;
				add_node_to_counter_list(from_node, to_node, markov_chain);
			}
		}
	}
	// free temp arr
	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		free(cells[i]);
	}
	return EXIT_SUCCESS;
}

static void free_if_not_null(void* block)
{
	if (block != NULL)
	{
		free(block);
	}
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
	if (argc != ALLOWED_ARGC)
	{
		printf(ARGUMENT_COUNT_ERROR_MESSAGE);
		return EXIT_FAILURE;
	}

#if OH_TESTS
	set_allocations(0);
#endif // OH_TESTS
	srand(str_to_int(argv[1]));
	
	markovChain markov_chain = load();
	if (markov_chain == NULL)
	{
		printf(ALLOCATION_ERROR_MASSAGE);
		return EXIT_FAILURE;
	}

	int routes = str_to_int(argv[2]);
	markovNode first = markov_chain->database->first->data;
	for (int i = 0; i < routes; i++)
	{
		printf("Random Walk %d: ", i + 1);
		generate_random_sequence(markov_chain, first, MAX_GENERATION_LENGTH);
		printf(NEW_LINE);
	}

	free_markov_chain(&markov_chain);

#ifdef OH_TESTS // debug
	printf("\n\nAllocations that didn't free: %llu\n", get_allocations());
#endif // OH_TESTS
	return EXIT_SUCCESS;
}
