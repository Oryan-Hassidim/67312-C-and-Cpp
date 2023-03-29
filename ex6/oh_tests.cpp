// C & C++ Course 67312
// ex6 Tests
// Oryan Hassidim
// Oryan.Hassidim@mail.huji.ac.il

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using std::string;
using std::cout;
using std::endl;


#define SPACES "        "
#define SUCCESS 1
#define FAILURE 0

#define red "\033[0;31m"
#define green "\033[0;32m"
#define regular "\033[0m"

typedef bool (*test_func)();
typedef void (*void_func)();

template <typename ErrorType>
bool test_error(void_func test)
{
	try
	{
		test();
		return false;
	}
	catch (const ErrorType&)
	{
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}

// 1


test_func tests[] =
{
	test_matrix_ctor_negative_or_zero_dimesions,
	test_matrix_brackets_index,
	test_matrix_arithmetics,
	test_matrix_loading_from_stream,
	test_dense_ctor_bias_non_vector,
	test_dense_ctor_different_rows,
	test_mlp_ctor_different_dimensions,
	test_mlp_ctor_non_4_arrays,
};



bool test(int test_number)
{
	int result = tests[test_number]();
	if (result)
	{
		cout << green << "Test " << test_number + 1 << " succeed :)" << regular << endl;
	}
	else
	{
		cout << red << "Test " << test_number + 1 << " failed :(" << regular << endl;
	}

	return result;
}


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
			cout << green <<
				"\n*************************\n\n  All tests succeed :)\n\n*************************"
				<< regular << endl;
		}
		else
		{
			cout << red <<
				"\n*****************************\n\n"
				<< tests_number - succeed << " out of " << tests_number <<
				" tests failed : (\n\n * ****************************"
				<< regular << endl;
		}
		return EXIT_SUCCESS;
	}

	int test_number = std::stoi(string(argv[1]));

	int result = test(test_number - 1);
	return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
