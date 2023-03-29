#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cipher.h"
#include "tests.h"

#define TEST_PARAMETERS_NUMBER 2
#define ENCODE_DECODE_PARAMETERS_NUMBER 5

#define ENCODE "encode"
#define DECODE "decode"
#define TEST "test"

#define LINE_LENGTH 1024

int
is_integer_number(const char str[])
{
	int i = 0;
	int have_digit = 0;
	if (str[0] == '-')
	{
		i++;
	}
	for (; str[i] != '\0'; i++)
	{
		if (str[i] < '0' || str[i] > '9')
		{
			return 0;
		}
		have_digit = 1;
	}
	return 1 && have_digit;
}

int
validate_args(int argc, char* argv[])
{
	if (argc != TEST_PARAMETERS_NUMBER
		&& argc != ENCODE_DECODE_PARAMETERS_NUMBER)
	{
		fprintf(stderr, "The program receives 1 or 4 arguments only.\n");
		return EXIT_FAILURE;
	}
	if (argc == TEST_PARAMETERS_NUMBER && strcmp(argv[1], TEST) != 0)
	{
		fprintf(stderr, "Usage: cipher test\n");
		return EXIT_FAILURE;
	}
	if (argc == ENCODE_DECODE_PARAMETERS_NUMBER)
	{
		if ((strcmp(argv[1], ENCODE) != 0 && strcmp(argv[1], DECODE) != 0))
		{
			fprintf(stderr, "The given command is invalid.\n");
			return EXIT_FAILURE;
		}
		if (!is_integer_number(argv[2]))
		{
			fprintf(stderr, "The given shift value is invalid.\n");
			return EXIT_FAILURE;
		}
		// validate input file exists
		FILE* input_file = fopen(argv[3], "r");
		if (input_file == NULL)
		{
			fprintf(stderr, "The given file is invalid.\n");
			return EXIT_FAILURE;
		}
		if (fclose(input_file) == EOF)
		{
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int
encode_decode(char command[], int shift, char input_file_name[],
	char output_file_name[])
{
	// select function to use
	void (*func) (char*, int) = encode;
	if (strcmp(command, DECODE) == 0)
	{
		func = decode;
	}

	// open files
	FILE* input_file = fopen(input_file_name, "r");
	FILE* output_file = fopen(output_file_name, "w");
	if (input_file == NULL || output_file == NULL)
	{
		// close the files
		if (input_file != NULL)
		{
			fclose(input_file);
		}
		if (output_file != NULL)
		{
			fclose(output_file);
		}

		fprintf(stderr, "The given file is invalid.\n");
		return EXIT_FAILURE;
	}

	// encode / decode
	char buffer[LINE_LENGTH + 1] = { 0 };
	while (fgets(buffer, LINE_LENGTH + 1, input_file) != NULL)
	{
		func(buffer, shift);
		fputs(buffer, output_file);
	}

	// close the files
	int closed = fclose(input_file) != 0;
	closed = closed || fclose(output_file) != 0;

	return EXIT_SUCCESS;
}

int
run_tests()
{
	// tests functions array
	int (*tests[]) ()
		= { test_encode_non_cyclic_lower_case_positive_k,
			test_encode_cyclic_lower_case_special_char_positive_k,
			test_encode_non_cyclic_lower_case_special_char_negative_k,
			test_encode_cyclic_lower_case_negative_k,
			test_encode_cyclic_upper_case_positive_k,
			test_decode_non_cyclic_lower_case_positive_k,
			test_decode_cyclic_lower_case_special_char_positive_k,
			test_decode_non_cyclic_lower_case_special_char_negative_k,
			test_decode_cyclic_lower_case_negative_k,
			test_decode_cyclic_upper_case_positive_k };

	int length = (int)(sizeof(tests) / sizeof(tests[0]));

	for (int i = 0; i < length; i++)
	{
		if (tests[i]())
		{
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int
main(int argc, char* argv[])
{
	// validate arguments
	if (validate_args(argc, argv) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	// tests
	if (argc == TEST_PARAMETERS_NUMBER)
	{
		return run_tests();
	}

	// encode or decode
	else if (argc == ENCODE_DECODE_PARAMETERS_NUMBER)
	{
		int k = (int)strtol(argv[2], NULL, 10);
		return encode_decode(argv[1], k, argv[3], argv[4]);
	}

	return EXIT_SUCCESS;
}
