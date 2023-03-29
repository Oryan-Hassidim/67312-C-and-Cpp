#ifdef OH_TESTS
#include "oh_tests/oh_tests_helpers.h"
#endif // OH_TESTS

#include "markov_chain.h"

#define SPACE " "

// ####################################################
//                  string helpers
// ####################################################

/**
* String equals function.
* @param s1 the first string.
* @param s2 the second string.
* @return true if equals, false otherwise.
*/
bool streq(str s1, str s2)
{
	return strcmp(s1, s2) == 0;
}

// ####################################################
//                  memory helpers
// ####################################################

/**
* Alocating new memory to string and copy the content of
* the given string.
* @param src source string.
* @return pointer to the new allocated string.
*/
str allocate_and_cpy(const str src)
{
	str dest = malloc(strlen(src) + 1);
	if (!dest)
	{
		return NULL;
	}
	strcpy(dest, src);
	return dest;
}

/**
 * Allocates new MarkovNode. Prints AllocationError when occurred.
 * @param data_ptr pointer to string to copy from.
 * @return pointer to the new node if succeed, NULL
 * if AllocationError occurred.
 */
markovNode allocate_new_node(str data_ptr)
{
	str new_str = allocate_and_cpy(data_ptr);
	if (new_str == NULL)
	{
		return NULL;
	}
	markovNode new_node = calloc(1, sizeof(struct MarkovNode));
	if (new_node == NULL)
	{
		free(new_str);
		return NULL;
	}
	new_node->counter_list = NULL;
	new_node->counter_list_size = 0;
	new_node->data = new_str;
	new_node->sum_of_frequencies = 0;
	return new_node;
}

// ####################################################
//                   list helpers
// ####################################################

/**
 * A helper function to find a node in a given index in a
 * linked list.
 * @param list the linked list to search in.
 * @param index the index to search.
 * @return pointer to the data's node if found, NULL otherwise.
 */
markovNode get_item(linkedList list, int index)
{
	index %= list->size;
	if (index == list->size - 1)
	{
		return list->last->data;
	}
	node cur = list->first;
	for (int i = 0; i < index; i++)
	{
		cur = cur->next;
	}
	return cur->data;
}

// ####################################################
//                   flow helpers
// ####################################################

/**
 * A helper function to print error message.
 * @param msg the error message to print.
 */
void print_error(str message)
{
	fprintf(stderr, message);
}

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number maximal number to return (not including)
 * @return Random number
 */
int get_random_number(int max_number)
{
	return rand() % max_number;
}

/**
 * inidicates if the given markov-node is a last node of a tweest.
 * @param node the node to check.
 * @return true if the node is a last node, false otherwise.
 */
bool node_is_end_of_tweet(markovNode node)
{
	return node->counter_list == NULL;
}

// ####################################################
//                     functions
// ####################################################

markovNode get_first_random_node(markovChain markov_chain)
{
	linkedList list = markov_chain->database;
	while (true) // until we will found
	{
		int n = get_random_number(list->size);
		markovNode node = get_item(list, n);
		if (!node_is_end_of_tweet(node)) // it is not end of tweet
		{
			return node;
		}
	}
}

markovNode get_next_random_node(markovNode state_struct_ptr)
{
	int r = get_random_number(state_struct_ptr->sum_of_frequencies);
	int i = 0;
	for (; r >= 0; i++)
	{
		r -= state_struct_ptr->counter_list[i].frequency;
	}
	return state_struct_ptr->counter_list[i - 1].markov_node;
}

void generate_random_sequence(markovChain markov_chain,
	markovNode first_node, int max_length)
{
	if (first_node == NULL)
	{
		first_node = get_first_random_node(markov_chain);
	}

	for (int i = 1; ; i++)
	{
		printf("%s", first_node->data);
		if (node_is_end_of_tweet(first_node) || i == max_length)
		{
			return;
		}
		printf(SPACE);
		first_node = get_next_random_node(first_node);
	}
}

void free_markov_chain(markovChain* ptr_chain)
{
	markovChain chain = *ptr_chain;
	node cur = chain->database->first, next = NULL;
	int size = chain->database->size;
	for (int i = 0; i < size; i++) // iterate over the list and free each item
	{
		next = cur->next;

		free(cur->data->counter_list);
		free(cur->data->data);
		free(cur->data);
		free(cur);

		cur = next;
	}

	free(chain->database);
	free(chain);
	*ptr_chain = NULL;
}

bool add_node_to_counter_list(markovNode first_node, markovNode second_node)
{
	// try find
	for (int i = 0; i < first_node->counter_list_size; i++)
	{
		if (first_node->counter_list[i].markov_node == second_node) // eq by ref
		{
			// add frequency
			first_node->counter_list[i].frequency++;
			first_node->sum_of_frequencies++;
			return true;
		}
	}

	// try allocate
	int new_size = sizeof(struct NextNodeCounter);
	new_size *= (1 + first_node->counter_list_size);
	nextNodeCounterArray new_list = first_node->counter_list == NULL ?
		calloc(1, new_size) :
		realloc(first_node->counter_list, new_size);

	if (new_list == NULL)
	{
		return false;
	}
	new_list[first_node->counter_list_size].markov_node = second_node;
	new_list[first_node->counter_list_size].frequency = 1;
	first_node->counter_list = new_list;
	first_node->counter_list_size++;
	first_node->sum_of_frequencies++;
	return true;
}

node get_node_from_database(markovChain markov_chain, str data_ptr)
{
	for (node cur = markov_chain->database->first;
		cur != NULL; cur = cur->next)
	{
		if (streq(cur->data->data, data_ptr))
		{
			return cur;
		}
	}
	return NULL;
}

node add_to_database(markovChain markov_chain, str data_ptr)
{
	// try find
	node cur = get_node_from_database(markov_chain, data_ptr);
	if (cur != NULL)
	{
		return cur;
	}

	// insert
	markovNode new_node = allocate_new_node(data_ptr);
	if (new_node == NULL || add(markov_chain->database, new_node) == 1)
	{ // allocation error
		return NULL;
	}
	return markov_chain->database->last;
}
