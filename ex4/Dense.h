#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

using std::domain_error;

typedef Matrix (* ActivationFunction)(const Matrix&);

/// <summary>
/// A dense layer in a neural network.
/// </summary>
class Dense
{
public:
	/// <summary>
	/// Constructs a dense layer.
	/// </summary>
	/// <param name="weights">The weights of the layer.</param>
	/// <param name="bias">The bias of the layer.</param>
	/// <param name="function">The activation function of the layer.</param>
	/// <exception cref="domain_error">Thrown if the bias is not a column vector.</exception>
	/// <exception cref="domain_error">Thrown if the weights and bias are not the same rows count.</exception>
	Dense(const Matrix& weights, const Matrix& bias,
		ActivationFunction function);

	/// <summary>
	/// Gets the weights of the layer.
	/// </summary>
	/// <returns>The weights of the layer.</returns>
	const Matrix& get_weights() const;
	/// <summary>
	/// Gets the bias of the layer.
	/// </summary>
	/// <returns>The bias of the layer.</returns>
	const Matrix& get_bias() const;
	/// <summary>
	/// Gets the activation function of the layer.
	/// </summary>
	/// <returns>The activation function of the layer.</returns>
	ActivationFunction get_activation() const;
	
	/// <summary>
	/// Applies the layer to the input.
	/// </summary>
	/// <param name="input">The input to the layer.</param>
	/// <returns>The output of the layer.</returns>
	Matrix operator()(const Matrix& input) const;
	
private:
	Matrix weights;
	Matrix bias;
	ActivationFunction activationFunction;
};
#endif //DENSE_H
