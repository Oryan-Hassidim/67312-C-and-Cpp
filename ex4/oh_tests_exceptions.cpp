// C & C++ Course 67312
// ex2 Tests
// Oryan Hassidim
// Oryan.Hassidim@mail.huji.ac.il

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Matrix.h"
#include "Dense.h"
#include "MlpNetwork.h"

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

bool test_domain_error(void_func test)
{
	try
	{
		test();
		return false;
	}
	catch (std::domain_error)
	{
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}
bool test_out_of_range_error(void_func test)
{
	try
	{
		test();
		return false;
	}
	catch (std::out_of_range)
	{
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}
bool test_runtime_error(void_func test)
{
	try
	{
		test();
		return false;
	}
	catch (std::runtime_error)
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
bool test_matrix_ctor_negative_or_zero_dimesions()
{
	return test_domain_error([]() { Matrix matrix(1, -1); })
		&& test_domain_error([]() { Matrix matrix(-1, 1); })
		&& test_domain_error([]() { Matrix matrix(0, 1); })
		&& test_domain_error([]() { Matrix matrix(1, 0); });
}
// 2
bool test_matrix_brackets_index()
{
	return test_out_of_range_error([]() { Matrix matrix; matrix[1] = 1; })
		&& test_out_of_range_error([]() { Matrix matrix; matrix[-1] = 1; })
		&& test_out_of_range_error([]() { Matrix matrix; matrix(0, 1) = 1; })
		&& test_out_of_range_error([]() { Matrix matrix; matrix(1, 0) = 1; })
		&& test_out_of_range_error([]() { const Matrix matrix; auto i = matrix[1]; })
		&& test_out_of_range_error([]() { const Matrix matrix; auto i = matrix[-1]; })
		&& test_out_of_range_error([]() { const Matrix matrix; auto i = matrix(0, 1); })
		&& test_out_of_range_error([]() { const Matrix matrix; auto i = matrix(1, 0); });
}
// 3
bool test_matrix_arithmetics()
{
	return test_domain_error([]() { Matrix m1; Matrix m2(1, 2); m1 + m2; })
		&& test_domain_error([]() { Matrix m1; Matrix m2(2, 1); m1* m2; })
		&& test_domain_error([]() { Matrix m1; Matrix m2(1, 2); m1 += m2; })
		&& test_domain_error([]() { Matrix m1; Matrix m2(1, 2); m1.dot(m2); });
}
// 4
bool test_matrix_loading_from_stream()
{
	std::stringstream s("12345678");
	Matrix m1, m2;
	s >> m1 >> m2;
	return m1[0] != m2[0]
		&& test_runtime_error([] {
		std::stringstream stream("1234");
	Matrix matrix(2, 3);
	stream >> matrix;
		})
		&& test_runtime_error([] {
			std::ifstream is;
		is.open("some_file_that_doesnt_exist.phjf", std::ios::in | std::ios::binary);
		Matrix matrix(2, 3);
		is >> matrix;
			});
}
// 5
bool test_dense_ctor_bias_non_vector()
{
	return test_domain_error([] { Dense d(Matrix(2, 2), Matrix(2, 2), activation::relu); });
}
// 6
bool test_dense_ctor_different_rows()
{
	return test_domain_error([] {Dense d(Matrix(2, 2), Matrix(3, 1), activation::relu); });
}
// 7
bool test_mlp_ctor_different_dimensions()
{
	return test_domain_error([] {
		Matrix weights[MLP_SIZE] = {
			Matrix(2, 2),
			Matrix(2, 2),
			Matrix(3, 3),
			Matrix(3, 3) };
	Matrix bias[MLP_SIZE] = {
		Matrix(2, 1),
		Matrix(2, 1),
		Matrix(3, 1),
		Matrix(3, 1) };
	MlpNetwork m(weights, bias); });
}
// 8
bool test_mlp_ctor_non_4_arrays()
{
	return test_domain_error([] {
		Matrix weights[MLP_SIZE] = {
			Matrix(2, 2),
			Matrix(2, 2),
			Matrix(2, 2) };
	Matrix bias[MLP_SIZE] = {
		Matrix(2, 1),
		Matrix(2, 1),
		Matrix(2, 1),
		Matrix(2, 1) };
	MlpNetwork m(weights, bias); });
}


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
