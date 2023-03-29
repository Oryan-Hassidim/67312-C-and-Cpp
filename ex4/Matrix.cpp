#include "Matrix.h"
#include <iostream>
#include <cmath>
#include <cstring>

#define TENTH 0.1F

using std::cout;
using std::swap;
using std::endl;
using std::streamsize;
using std::streampos;
using std::istream;

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols) {
	if (rows < 1)
	{
		throw std::domain_error("Matrix rows must be greater than 0.");
	}
	if (cols < 1)
	{
		throw std::domain_error("Matrix cols must be greater than 0.");
	}
	data = new float[rows * cols];
	std::fill_n(data, rows * cols, 0.0F);
	transposed = false;
}

Matrix::Matrix() : Matrix(1, 1)
{
}

Matrix::Matrix(const Matrix& matrix)
{
	rows = matrix.rows;
	cols = matrix.cols;
	transposed = matrix.transposed;
	this->data = new float[rows * cols];
	std::memcpy(data, matrix.data, (size_t)rows * cols * sizeof(float));
}

Matrix::~Matrix()
{
	delete[] data;
}

int Matrix::get_rows() const
{
	return rows;
}

int Matrix::get_cols() const
{
	return cols;
}

Matrix& Matrix::transpose()
{
	transposed = !transposed;
	swap(rows, cols);
	return *this;
}

Matrix& Matrix::vectorize()
{
	if (transposed && rows != 1)
	{
		float* new_data = new float[rows * cols];
		for (int i = 0; i < rows * cols; i++)
		{
			new_data[i] = data[i];
		}
		delete[] data;
		data = new_data;
		transposed = false;
	}
	rows *= cols;
	cols = 1;
	return *this;
}

void Matrix::plain_print() const
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cout << (*this)(i, j) << " ";
		}
		cout << endl;
	}
}

Matrix Matrix::dot(const Matrix& other) const
{
	if (cols != other.cols || rows != other.rows)
	{
		throw std::domain_error("Matrices must have the same dimensions");
	}
	Matrix result(cols, rows);
	for (int i = 0; i < rows * cols; i++)
	{
		result[i] = (*this)[i] * other[i];
	}
	return result;
}

float Matrix::sum() const
{
	float sum = 0;
	for (int i = 0; i < rows * cols; i++)
	{
		sum += data[i];
	}
	return sum;
}

float Matrix::norm() const
{
	float sum = 0;
	for (int i = 0; i < rows * cols; i++)
	{
		sum += data[i] * data[i];
	}
	return sqrt(sum);
}

int Matrix::argmax() const
{
	int max_index = 0;
	for (int i = 1; i < rows * cols; i++)
	{
		if (data[i] > data[max_index])
		{
			max_index = i;
		}
	}
	return max_index;
}

Matrix& Matrix::operator+=(const Matrix& matrix)
{
	if (rows != matrix.rows || cols != matrix.cols)
	{
		throw std::domain_error("Matrix addition: incompatible dimensions");
	}
	for (int i = 0; i < rows * cols; i++)
	{
		float& t = (*this)[i];
		t += matrix[i];
	}
	return *this;
}

Matrix Matrix::operator+(const Matrix& matrix) const
{
	if (rows != matrix.rows || cols != matrix.cols)
	{
		throw std::domain_error("Matrix addition: incompatible dimensions");
	}
	Matrix result(rows, cols);
	for (int i = 0; i < rows * cols; i++)
	{
		result[i] = (*this)[i] + matrix[i];
	}
	return result;
}

Matrix& Matrix::operator=(const Matrix& matrix)
{
	if (this == &matrix)
	{
		return *this;
	}
	// copy and swap
	Matrix temp(matrix);
	swap(rows, temp.rows);
	swap(cols, temp.cols);
	swap(data, temp.data);
	swap(transposed, temp.transposed);
	return *this;
}

Matrix Matrix::operator*(const Matrix& other) const
{
	Matrix result(rows, other.cols);
	if (cols != other.rows)
	{
		throw std::domain_error("Matrix dot product: incompatible dimensions");
	}
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < other.cols; j++)
		{
			float sum = 0;
			for (int k = 0; k < cols; k++)
			{
				sum += (*this)(i, k) * other(k, j);
			}
			result(i, j) = sum;
		}
	}
	return result;
}

Matrix Matrix::operator*(float scalar) const
{
	Matrix result(rows, cols);
	for (int i = 0; i < rows * cols; i++)
	{
		result[i] = (*this)[i] * scalar;
	}
	return result;
}

float& Matrix::operator()(int row, int col)
{
	if (col < 0 || col >= cols || row < 0 || row >= rows)
	{
		throw std::out_of_range("Matrix index out of bounds");
	}
	return (*this)[row * cols + col];
}

float Matrix::operator()(int row, int col) const
{
	if (col < 0 || col >= cols || row < 0 || row >= rows)
	{
		throw std::out_of_range("Matrix index out of bounds");
	}
	return (*this)[row * cols + col];
}

float Matrix::operator[](int arg) const
{
	if (arg < 0 || arg >= rows * cols)
	{
		throw std::out_of_range("Matrix index out of bounds.");
	}
	return transposed ? data[(arg % cols) * rows + (arg / cols)] : data[arg];
}

float& Matrix::operator[](int arg)
{
	if (arg < 0 || arg >= rows * cols)
	{
		throw std::out_of_range("Matrix index out of bounds.");
	}
	return transposed ? data[(arg % cols) * rows + (arg / cols)] : data[arg];
}

Matrix operator*(float scalar, const Matrix& matrix)
{
	return matrix * scalar;
}

ostream& operator<<(ostream& os, const Matrix& matrix)
{
	for (int i = 0; i < matrix.rows; i++)
	{
		for (int j = 0; j < matrix.cols; j++)
		{
			os << (matrix(i, j) > TENTH ? "**" : "  ");
		}
		os << endl;
	}
	return os;
}

istream& operator>>(istream& is, Matrix& matrix)
{
	streamsize size = (streamsize)matrix.rows * matrix.cols * sizeof(float);
	char* buffer = (char*)matrix.data;
	try
	{
		// check if the size of the stream is enugh to read the matrix
		if (!is.good())
		{
			throw std::runtime_error("Stream is not good.");
		}
		streampos start = is.tellg();
		is.seekg(0, istream::end);
		streampos end = is.tellg();
		is.seekg(start);
		if (end - is.tellg() < size)
		{
			throw std::runtime_error("Not enough data to read the matrix");
		}

		is.read(buffer, size);
		if (is.gcount() != size || !is)
		{
			throw std::runtime_error("Matrix input: not enough data");
		}
	}
	catch (const std::runtime_error& e)
	{
		throw e;
	}
	catch (const std::exception&)
	{
		throw std::runtime_error("Error reading matrix from file.");
	}
	matrix.transposed = false;
	return is;
}
