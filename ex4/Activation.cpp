#include "Activation.h"
#include <cmath>

using std::exp;
using std::max;

Matrix activation::relu(const Matrix& matrix)
{
	Matrix result(matrix);
	int size = matrix.get_rows() * matrix.get_cols();
	for (int i = 0; i < size; i++)
	{
		result[i] = result[i] > 0 ? result[i] : 0;
	}
	return result;
}

Matrix activation::softmax(const Matrix& matrix)
{
	Matrix result(matrix);
	int size = matrix.get_rows() * matrix.get_cols();
	float sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum += exp(result[i]);
	}
	for (int i = 0; i < size; i++)
	{
		result[i] = exp(result[i]) / sum;
	}
	return result;
}
