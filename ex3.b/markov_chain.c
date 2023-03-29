#ifdef OH_TESTS
#ifdef _WIN32
#include "oh_tests/oh_tests_helpers.h"
#else // linux
#include "/cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/oh_tests/oh_tests_helpers.h"
#endif //_WIN32
#endif // OH_TESTS

#include "markov_chain.h"

// ####################################################
//                  memory helpers
// ####################################################

/**
 * Allocates new MarkovNode.
 * @param data_ptr pointer to string to copy from.
 * @return pointer to the new node if succeed, NULL
 * if AllocationError occurred.
 */
markovNode allocate_new_node(T data_ptr, markovChain markov_chain)
{
	T new_data = markov_chain->copy_func(data_ptr);
	if (new_data == NULL)
	{
		return NULL;
	}
	markovNode new_node = calloc(1, sizeof(struct MarkovNode));
	if (new_node == NULL)
	{
		markov_chain->free_data(new_data);
		return NULL;
	}
	new_node->counter_list = NULL;
	new_node->counter_list_size = 0;
	new_node->data = new_data;
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

bool is_last_node(markovNode node, markovChain markov_chain)
{
	return node->counter_list == NULL || markov_chain->is_last(node->data);
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
		if (!is_last_node(node, markov_chain)) // it is not end of tweet
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

void generate_random_sequence(markovChain markov_chain, markovNode first_node,
	int max_length)
{
	if (first_node == NULL)
	{
		first_node = get_first_random_node(markov_chain);
	}

	for (int i = 1;; i++)
	{
		markov_chain->print_func(first_node->data);
		if (is_last_node(first_node, markov_chain) || i == max_length)
		{
			return;
		}
		first_node = get_next_random_node(first_node);
	}
}

void free_block_if_not_null(void* block)
{
	if (block != NULL)
	{
		free(block);
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

		free_block_if_not_null(cur->data->counter_list);
		chain->free_data(cur->data->data);
		free_block_if_not_null(cur->data);
		free_block_if_not_null(cur);

		cur = next;
	}

	free_block_if_not_null(chain->database);
	free_block_if_not_null(chain);
	*ptr_chain = NULL;
}

bool add_node_to_counter_list(markovNode first_node, markovNode second_node,
	markovChain markov_chain)
{
	if (markov_chain == NULL) {
	}
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
	nextNodeCounterArray new_list = realloc(first_node->counter_list, new_size);

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

node get_node_from_database(markovChain markov_chain, T data_ptr)
{
	for (node cur = markov_chain->database->first; cur != NULL; cur = cur->next)
	{
		if (markov_chain->comp_func(cur->data->data, data_ptr) == 0)
		{
			return cur;
		}
	}
	return NULL;
}

node add_to_database(markovChain markov_chain, T data_ptr)
{
	// try find
	node cur = get_node_from_database(markov_chain, data_ptr);
	if (cur != NULL)
	{
		return cur;
	}

	// insert
	markovNode new_node = allocate_new_node(data_ptr, markov_chain);
	if (new_node == NULL) // allocation error
	{
		return NULL;
	}
	if (add(markov_chain->database, new_node) == 1) // allocation error
	{
		markov_chain->free_data(new_node->data);
		free(new_node);
		return NULL;
	}
	return markov_chain->database->last;
}
