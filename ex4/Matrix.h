#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
using std::ostream;
using std::istream;

class Matrix {
private:
	int rows;
	int cols;
	bool transposed;
	float* data;
public:
	/**
	 * @struct Dims
	 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
	 */
	struct dims {
		int rows, cols;
	};
	
	/// <summary>
	/// Constructs Matrix of size rows * cols. Inits all
	/// elements to 0.
	/// </summary>
	/// <param name="rows">Number of rows</param>
	/// <param name="cols">Number of columns</param>
	Matrix(int rows, int cols);
	/// <summary>
	/// Constructs Matrix of size 1 * 1. Inits all
	/// element to value 0.
	/// </summary>
	Matrix();
	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="matrix">Matrix to copy</param>
	Matrix(const Matrix& matrix);

	~Matrix();

	/// <summary>
	/// Returns the number of rows in the matrix.
	/// </summary>
	/// <returns>Number of rows in the matrix.</returns>
	int get_rows() const;
	/// <summary>
	/// Returns the number of columns in the matrix.
	/// </summary>
	/// <returns>Number of columns in the matrix.</returns>
	int get_cols() const;

	/// <summary>
	/// Transforms a matrix into its transpose matrix.
	/// </summary>
	/// <returns>the given matrix</returns>
	Matrix& transpose();
	/// <summary>
	/// Transforms a matrix into a column vector.
	/// </summary>
	/// <returns>the given matrix</returns>
	Matrix& vectorize();

	/// <summary>
	/// Prints matrix elements, no return value.
	/// </summary>
	void plain_print() const;

	/// <summary>
	/// Returns a new matrix which is the element-wise multiplication
	/// (Hadamard product) of this matrix and another matrix.
	/// </summary>
	/// <param name="other">the second matrix</param>
	/// <returns>the multiplication result</returns>
	Matrix dot(const Matrix& other) const;

	/// <summary>
	/// Returns the sum of the matrix.
	/// </summary>
	/// <returns>the sum of the matrix</returns>
	float sum() const;
	/// <summary>
	/// Returns the Frobenius norm of the given matrix.
	/// </summary>
	/// <returns>the Frobenius norm of the given matrix</returns>
	float norm() const;
	/// <summary>
	/// Returns the brackets index of the largest
	/// number in the matrix
	/// </summary>
	/// <returns>the brackets index of the largest number in the matrix</returns>
	int argmax() const;

	/// <summary>
	/// Matrix addition accumulation.
	/// </summary>
	/// <param name="matrix">the matrix to add</param>
	/// <returns>the given matrix</returns>
	Matrix& operator+=(const Matrix& matrix);
	/// <summary>
	/// Matrix addition.
	/// </summary>
	/// <param name="matrix">the matrix to add</param>
	/// <returns>new matrix which is the sum of the two matrices</returns>
	Matrix operator+(const Matrix& matrix) const;
	/// <summary>
	/// Assignment.
	/// </summary>
	/// <param name="matrix">the matrix to assign</param>
	/// <returns>the given matrix</returns>
	Matrix& operator=(const Matrix& matrix);
	/// <summary>
	/// Matrix multiplication.
	/// </summary>
	/// <param name="matrix">the matrix to multiply</param>
	/// <returns>new matrix which is the product of the two matrices</returns>
	Matrix operator*(const Matrix& matrix) const;
	/// <summary>
	/// Matrix multiplication by a scalar.
	/// </summary>
	/// <param name="scalar">the scalar to multiply</param>
	/// <returns>new matrix which is the product of the matrix and the scalar</returns>
	Matrix operator*(float scalar) const;
	/// <summary>
	/// Matrix multiplication by a scalar.
	/// </summary>
	/// <param name="scalar">the scalar to multiply</param>
	/// <returns>new matrix which is the product of the matrix and the scalar</returns>
	friend Matrix operator*(float scalar, const Matrix& matrix);

	/// <summary>
	/// Matrix 2D element access.
	/// </summary>
	/// <param name="row">the row index</param>
	/// <param name="col">the column index</param>
	/// <returns>the element at the given row and column</returns>
	float& operator()(int row, int col);
	/// <summary>
	/// Matrix 2D element const access.
	/// </summary>
	/// <param name="row">the row index</param>
	/// <param name="col">the column index</param>
	/// <returns>the element at the given row and column</returns>
	float operator()(int row, int col) const;
	/// <summary>
	/// Matrix 1D element access.
	/// </summary>
	/// <param name="index">the index</param>
	/// <returns>the element at the given index</returns>
	float& operator[](int arg);
	/// <summary>
	/// Matrix 1D element const access.
	/// </summary>
	/// <param name="index">the index</param>
	///	<returns>the element at the given index</returns>
	float operator[](int arg) const;

	/// <summary>
	/// Matrix stream output.
	/// </summary>
	/// <param name="os">the output stream</param>
	/// <param name="matrix">the matrix to output</param>
	/// <returns>the output stream</returns>
	friend ostream& operator<<(ostream& os, const Matrix& matrix);
	/// <summary>
	/// Matrix stream input.
	/// </summary>
	/// <param name="is">the input stream</param>
	///	<param name="matrix">the matrix to input</param>
	/// <returns>the input stream</returns>
	friend istream& operator>>(istream& is, Matrix& matrix);
};
#endif //MATRIX_H
