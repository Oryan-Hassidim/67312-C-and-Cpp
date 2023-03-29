#include "Matrix.h"

#ifndef ACTIVATION_H
#define ACTIVATION_H

/// <summary>
/// Activation functions.
/// </summary>
namespace activation
{
	/// <summary>
	/// ReLU activation function.
	/// </summary>
	/// <param name="matrix">Input matrix to apply the function on.</param>
	/// <returns>new matrix of applying ReLU on the given matrix.</returns>
	Matrix relu(const Matrix& matrix);
	
	/// <summary>
	/// SoftMax activation function.
	/// </summary>
	/// <param name="matrix">Input matrix to apply the function on.</param>
	/// <returns>new matrix of applying SoftMax on the given matrix.</returns>
	Matrix softmax(const Matrix& matrix);
}

#endif //ACTIVATION_H