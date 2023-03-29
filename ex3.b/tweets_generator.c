#ifdef OH_TESTS
#ifdef _WIN32
#include "oh_tests/oh_tests_helpers.h"
#else // linux
#include "/cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/oh_tests/oh_tests_helpers.h"
#endif //_WIN32
#endif // OH_TESTS

#include "markov_chain.h"
#include <limits.h>
#include <string.h>

#define ARGUMENT_COUNT_ERROR_MESSAGE \
  "USAGE: tweets_generator <seed>" \
  " <tweets count> <text corpus path> [<words to read>]\n"
#define FILE_OPEN_ERROR_MESSAGE "Error: cannot access the file %s\n"

#define DOT '.'
#define TWEET_TITLE "Tweet %d: "
#define ALLOWED_ARGUMENTS_COUNT_1 4
#define ALLOWED_ARGUMENTS_COUNT_2 5
#define MAX_WORDS_TO_TWEET 20
#define DELIMITERS " \r\n"
#define NEW_LINE "\n"
#define MAX_WORD 128
#define MAX_LINE 1000

// ####################################################
//                  string helpers
// ####################################################

typedef char* str;

static bool is_end_of_tweet(const str word)
{
	return word[strlen(word) - 1] == DOT;
}

static int str_to_int(const str s)
{
	return strtol(s, NULL, 10);
}

static bool word_is_last(const T word)
{
	return is_end_of_tweet(word);
}

static void word_print(const T word)
{
	printf("%s%s", (str)word, word_is_last(word) ? "" : " ");
}

static int word_compare(const T w1, const  T w2)
{
	return strcmp(w1, w2);
}

static void word_free(T word)
{
	free(word);
}

/**
* Alocating new memory to string and copy the content of
* the given string.
* @param src source string.
* @return pointer to the new allocated string.
*/
static str allocate_and_cpy(const str src)
{
	str dest = malloc(strlen(src) + 1);
	if (!dest)
	{
		return NULL;
	}
	strcpy(dest, src);
	return dest;
}

static T word_copy(T word)
{
	return allocate_and_cpy((str)word);
}

// ####################################################
//                  debug helpers
// ####################################################

#ifdef OH_TESTS
/**
 * a Helper function to print a markov-chain to stdout with format:
 * <number>. <word> : [ { <word> : <frequency> } { <word> : <frequency> }...]
 * @param markov_chain the markov-chain to print.
 */
static void print_markov_chain(markovChain markov_chain)
{
	printf("MarkovChain:\n");
	linkedList list = markov_chain->database;
	node cur = list->first;
	for (int i = 0; i < list->size; i++)
	{
		printf("%d.\t%s : [ ", i, (str)cur->data->data);
		nextNodeCounterArray arr = cur->data->counter_list;
		for (int j = 0; j < cur->data->counter_list_size; j++)
		{
			printf("{%s : %d} ", (str)arr[j].markov_node->data,
				arr[j].frequency);
		}
		printf("]\n");
		cur = cur->next;
	}
}
#endif // OH_TESTS

// ####################################################
//                   flow helpers
// ####################################################

/**
 * validates the args count. if not valid - prints to stdout and returns false.
 * @param argc the args count
 * @return true if valid, false otherwise.
 */
static bool validate(int argc)
{
	if (argc != ALLOWED_ARGUMENTS_COUNT_1 && argc != ALLOWED_ARGUMENTS_COUNT_2)
	{
		printf(ARGUMENT_COUNT_ERROR_MESSAGE);
		return false;
	}
	return true;
}

/**
 * a Helper free allocated memory after an error occurred.
 * @param ptr the pointer to free.
 */
static void free_if_not_null(void* ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
	}
}

/**
 * Fills the database with the tweets file tweets.
 * On allocation error - stops the operation without free
 * and without printing to stderr / stdout.
 * @param fp pointer to the opened file
 * @param words_to_read
 * @param markov_chain a markov_chain to fill into
 * @return EXIT_SUCCESS if succeeded, EXIT_FAILURE otherwise
 */
static int fill_database(FILE* fp, int words_to_read, markovChain markov_chain)
{
	int i = 0;
	char buffer[MAX_LINE] = { 0 };
	// Lines
	markovNode prev_node = NULL;
	node new_node = NULL;
	while (!feof(fp))
	{
		fgets(buffer, MAX_LINE, fp);
		// words
		str word = strtok(buffer, DELIMITERS);
		while (word)
		{
			new_node = add_to_database(markov_chain, word);
			if (new_node == NULL) // allocation error
			{
				return EXIT_FAILURE;
			}
			if (prev_node != NULL) // not start of a tweet
			{                    // add to prev_node's counter_list
				if (!add_node_to_counter_list(prev_node, new_node->data,
					markov_chain))
				{ // allocation error
					return EXIT_FAILURE;
				}
			}
			if (is_end_of_tweet(word)) // end of tweet
			{
				prev_node = NULL;
			}
			else // not end of tweet
			{
				prev_node = new_node->data;
			}
			if (++i == words_to_read) // max words
			{
				return EXIT_SUCCESS;
			}
			word = strtok(NULL, DELIMITERS);
		}
	}
	return EXIT_SUCCESS;
}

/**
 * Loading the database from the file. Prints to stdout if failed.
 * @param
 * @return a new markov_chain if succeeded, NULL otherwise.
 */
static markovChain load(str file_name, int words_to_read)
{
	FILE* f = fopen(file_name, "r");
	if (f == NULL)
	{
		printf(FILE_OPEN_ERROR_MESSAGE, file_name);
		return NULL;
	}

	// allocate the chain and the linked_list
	markovChain markov_chain = calloc(1, sizeof(struct MarkovChain));
	linkedList list = calloc(1, sizeof(struct LinkedList));
	if (markov_chain == NULL || list == NULL)
	{
		printf("%s", ALLOCATION_ERROR_MASSAGE);
		free_if_not_null(markov_chain);
		free_if_not_null(list);
		fclose(f);
		return NULL;
	}

	markov_chain->database = list;
	markov_chain->comp_func = word_compare;
	markov_chain->copy_func = word_copy;
	markov_chain->free_data = word_free;
	markov_chain->is_last = word_is_last;
	markov_chain->print_func = word_print;

	if (fill_database(f, words_to_read, markov_chain) == EXIT_FAILURE)
	{
		fprintf(stderr, ALLOCATION_ERROR_MASSAGE);
		free_markov_chain(&markov_chain);
		fclose(f);
		return NULL;
	}
	fclose(f);

	return markov_chain;
}

/**
 * prints to stdout generated tweets.
 * @param markov_chain the markov_chain to generate tweets from.
 * @param count the count of tweets to generate.
 */
static void print_tweets(markovChain markov_chain, int count)
{
	for (int i = 0; i < count; i++)
	{
		printf(TWEET_TITLE, i + 1);
		generate_random_sequence(markov_chain, NULL, MAX_WORDS_TO_TWEET);
		printf(NEW_LINE);
	}
}

int main(int argc, str argv[])
{
#ifdef OH_TESTS // debug
	set_allocations(0);
#endif // OH_TESTS

	if (!validate(argc)) // validating the args
	{
		return EXIT_FAILURE;
	}

	// taking the arg's values
	srand(str_to_int(argv[1])); // initialize seed
	int words_to_read = argc == 5 ? str_to_int(argv[4]) : INT_MAX;
	markovChain markov_chain = load(argv[3], words_to_read);

	if (markov_chain == NULL) // failure
	{
		return EXIT_FAILURE;
	}

#ifdef OH_TESTS // debug
	print_markov_chain(markov_chain);
	printf("\n\n");
#endif // OH_TESTS

	print_tweets(markov_chain, str_to_int(argv[2]));

	free_markov_chain(&markov_chain);

#ifdef OH_TESTS // debug
	printf("\n\nAllocations that didn't free: %llu\n", get_allocations());
#endif // OH_TESTS
	return EXIT_SUCCESS;
}
