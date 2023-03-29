#include "MlpNetwork.h"
#include "Activation.h"
#include <string>

using std::to_string;

MlpNetwork::MlpNetwork(const Matrix weights[MLP_SIZE], const Matrix bias[MLP_SIZE])
	: layers{
		Dense(weights[0], bias[0], activation::relu),
		Dense(weights[1], bias[1], activation::relu),
		Dense(weights[2], bias[2], activation::relu),
		Dense(weights[3], bias[3], activation::softmax) }
{
	for (int i = 1; i < MLP_SIZE; i++)
	{
		if (layers[i - 1].get_bias().get_rows() != layers[i].get_weights().get_cols())
		{
			throw domain_error("dense[" + to_string(i - 1) + "] dimensions are not"
				" compatible with dense[" + to_string(i) + "] dimensions.");
		}
	}
}

digit MlpNetwork::operator()(const Matrix& img) const
{
	Matrix result = img;
	result.vectorize();
	for (int i = 0; i < MLP_SIZE; i++)
	{
		result = layers[i](result);
	}
	int arg_max = result.argmax();
	return { (unsigned int)arg_max, result[arg_max] };
}
