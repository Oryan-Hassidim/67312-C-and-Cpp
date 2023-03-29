#include "Dense.h"

using std::domain_error;

Dense::Dense(const Matrix& weights, const Matrix& bias,
	ActivationFunction function)
	: weights(weights), bias(bias), activationFunction(function)
{
	if (bias.get_cols() != 1)
	{
		throw domain_error("Bias must be a column vector.");
	}
	if (weights.get_rows() != bias.get_rows())
	{
		throw domain_error("Bias must have the same number of"
			" rows as the weights.");
	}
}

const Matrix& Dense::get_weights() const
{
	return weights;
}

const Matrix& Dense::get_bias() const
{
	return bias;
}

ActivationFunction Dense::get_activation() const
{
	return activationFunction;
}

Matrix Dense::operator()(const Matrix& input) const
{
	return activationFunction((weights * input) + bias);
}
