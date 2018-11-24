#ifndef NEURALNETWORK
#define NEURALNETWORK
#include "matrix.cpp"
#include <vector>
#include <cmath>
#include "approx.cpp"
#include "activators.cpp"

template<typename T>
class NeuralNetwork : public Approximator<T> {

	private:

		std::vector<Matrix<T>> weights;
		std::vector<Matrix<T>> biases;
		size_t input_size;
		size_t output_size;
		Activation activationType;

	public:

		NeuralNetwork(std::vector<size_t> sizes, Activation type) {
			activationType = type;
			input_size = sizes[0];
			output_size = *sizes.rbegin();
			for (auto p = sizes.begin(), c = sizes.begin() + 1; c < sizes.end(); c++, p++) {
				biases.push_back(Matrix<T>::initializeRandom(*c, 1));
				weights.push_back(Matrix<T>::initializeRandom(*c, *p));
			}
		}

		Matrix<T> evaluate(Matrix<T> m){
			for (auto w = weights.begin(), b = biases.begin(); w < weights.end(); w++, b++){
				m = Activator<T>::activation((*w ^ m) + *b, activationType);
			}
			return m;
		}

		std::vector<Matrix<T>> playback(Matrix<T> m){
			std::vector<Matrix<T>> backwards;
			backwards.push_back(m);
			for (auto w = weights.begin(), b = biases.begin(); w < weights.end(); w++, b++){
				m = Activator<T>::activation((*w ^ m) + *b, activationType);
				backwards.push_back(m);
			}
			return backwards;
		}

		void update(Matrix<T>& data, Matrix<T>& target) {
			std::vector<Matrix<T>> per_layer_calculated = playback(data);
			Matrix<T> error_derivative = *per_layer_calculated.rbegin() - target;
		}
};

#endif
