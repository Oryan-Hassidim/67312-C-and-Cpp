#ifndef _MARKOV_CHAIN_H
#define _MARKOV_CHAIN_H

#include "linked_list.h"
#include <stdbool.h> // for bool
#include <stdio.h>   // For printf(), sscanf()
#include <stdlib.h>  // For exit(), malloc()

#define ALLOCATION_ERROR_MASSAGE \
  "Allocation failure: Failed to allocate new memory\n"


/***************************/
/*   insert typedefs here  */
/***************************/

/***************************/

// I very very hate *, so I using lowercase for pointers.
// for call to the struct itself, use "struct" prefix and uppercase.
typedef struct Node* node;
typedef struct LinkedList* linkedList;
typedef void* T;
typedef struct NextNodeCounter* nextNodeCounterArray;
typedef struct MarkovNode* markovNode;
typedef struct MarkovChain* markovChain;
typedef void (*generic_print) (T);
typedef int (*generic_compare) (T, T);
typedef void (*generic_free) (T);
typedef T(*generic_copy) (T);
typedef bool (*generic_is_last) (T);

/***************************/
/*        STRUCTS          */
/***************************/

#pragma pack(push, 1)
typedef struct MarkovNode
{
	//                                     Bytes
	//                                     -----
	T data;                            //  0-7
	nextNodeCounterArray counter_list; //  8-15
	int counter_list_size;             //  16-19
	int sum_of_frequencies;            //  20-23
} MarkovNode;

typedef struct NextNodeCounter
{
	//									   Bytes
	//									   -----
	markovNode markov_node; //  0-7
	int frequency;          //  8-11
} NextNodeCounter;
#pragma pack(pop)

/* DO NOT ADD or CHANGE variable names in this struct */
typedef struct MarkovChain
{
	linkedList database;

	// pointer to a func that receives data from a generic type and prints it
	// returns void.
	generic_print print_func;

	// pointer to a func that gets 2 pointers of generic data type(same one) and
	// compare between them */
	// returns: - a positive value if the first is bigger
	//          - a negative value if the second is bigger
	//          - 0 if equal
	generic_compare comp_func;

	// a pointer to a function that gets a pointer of generic data type and frees
	// it.
	// returns void.
	generic_free free_data;

	// a pointer to a function that  gets a pointer of generic data type and
	// returns a newly allocated copy of it
	// returns a generic pointer.
	generic_copy copy_func;

	//  a pointer to function that gets a pointer of generic data type and
	//  returns:
	//      - true if it's the last state.
	//      - false otherwise.
	generic_is_last is_last;
} MarkovChain;

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
markovNode get_first_random_node(markovChain markov_chain);

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
markovNode get_next_random_node(markovNode state_struct_ptr);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with, if NULL- choose a random
 * markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_random_sequence(markovChain markov_chain, markovNode first_node,
	int max_length);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain(markovChain* markov_chain);

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool add_node_to_counter_list(markovNode first_node, markovNode second_node,
	markovChain markov_chain);

/**
* Check if data_ptr is in database. If so, return the markov_node wrapping it
* in
 * the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
node get_node_from_database(markovChain markov_chain, T data_ptr);

/**
* If data_ptr in markov_chain, return it's node. Otherwise, create new
 * node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return node wrapping given data_ptr in given chain's database
 */
node add_to_database(markovChain markov_chain, T data_ptr);

#endif /* MARKOV_CHAIN_H */
