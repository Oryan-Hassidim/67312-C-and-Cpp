//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"

#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit {
	unsigned int value;
	float probability;
} digit;

const Matrix::dims img_dims = {28, 28};
const Matrix::dims weights_dims[] = {{128, 784},
									 {64,  128},
									 {20,  64},
									 {10,  20}};
const Matrix::dims bias_dims[] = {{128, 1},
								  {64,  1},
								  {20,  1},
								  {10,  1}};

class MlpNetwork
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="MlpNetwork"/> class.
	/// </summary>
	/// <param name="weights">The weights matrices of the network.</param>
	/// <param name="bias">The bias matrices of the network.</param>
	/// <exception cref="domain_error">Thrown if the layers output and
	/// input from different dimensions.</exception>
	MlpNetwork(const Matrix weights[MLP_SIZE], const Matrix bias[MLP_SIZE]);
	
	/// <summary>
	/// apply the network to the input image.
	/// </summary>
	/// <param name="img">The input image.</param>
	/// <returns>the identified digit</returns>
	digit operator()(const Matrix& img) const;
private:
	Dense layers[MLP_SIZE];
};
#endif // MLPNETWORK_H
