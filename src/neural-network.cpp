#ifndef NEURALNETWORK
#define NEURALNETWORK
#include "matrix.cpp"
#include <vector>
#include "approx.cpp"

template<typename T>
class NeuralNetwork : public Approximator<T> {

	private:

		std::vector<Matrix<T>> weights;
		std::vector<Matrix<T>> biases;
		size_t input_size;
		size_t output_size;

	public:

		NeuralNetwork(std::vector<size_t> sizes) {
			input_size = sizes[0];
			output_size = *sizes.rbegin();
			for (auto p = sizes.begin(), c = sizes.begin() + 1; c < sizes.end(); c++, p++) {
				biases.push_back(Matrix<T>::initializeRandom(*c, 1));
				weights.push_back(Matrix<T>::initializeRandom(*c, *p));
			}
		}
	
		Matrix<T> evaluate(Matrix<T> m){
			for (auto w = weights.begin(), b = biases.begin(); w < weights.end(); w++, b++)
				m = (*w ^ m) + *b;
			return m;
		}

		void update(Matrix<T>& data, Matrix<T>& target) {

		}	
};

#endif
