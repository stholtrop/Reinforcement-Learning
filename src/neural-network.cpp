#ifndef NEURALNETWORK
#define NEURALNETWORK
#include "matrix.cpp"
#include <vector>
#include <algorithm>
#include <cmath>
#include "approx.cpp"
#include "activators.cpp"
#include <functional>

template<typename T>
class NeuralNetwork : public Approximator<T> {

	private:

		std::vector<Matrix<T>> weights;
		std::vector<Matrix<T>> biases;

		size_t inputSize;
		size_t outputSize;

		std::function<Matrix<T>(const Matrix<T>&)> activation;
		std::function<Matrix<T>(const Matrix<T>&)> derivative;

	public:

		NeuralNetwork(std::vector<size_t> sizes, const Function<T>& av) {

			activation = Matrix<T>::wrap([&av] (const T x) { return av.function(x);});
			derivative = Matrix<T>::wrap([&av] (const T x) { return av.derivative(x);});
			inputSize = sizes[0];
			outputSize = *sizes.rbegin();

			for (auto p = sizes.begin(), c = sizes.begin() + 1; c < sizes.end(); c++, p++) {
				biases.push_back(Matrix<T>::initializeRandom(*c, 1));
				weights.push_back(Matrix<T>::initializeRandom(*c, *p));
			}
		}


		Matrix<T> evaluate(Matrix<T> m){
			
			for (auto w = weights.begin(), b = biases.begin(); w < weights.end(); w++, b++) {
				std::cout << w->dimensions() << " ^ " << m.dimensions() << " + " << b->dimensions() << std::endl;
				std::cout << *w << std::endl << m << std::endl << *b << std::endl;
				m = activation((*w ^ m) + *b);
				std::cout << "New m: " << m.dimensions() << std::endl;
			}

			return m;
		}


		std::vector<Matrix<T>> playback(Matrix<T> m) {

			std::vector<Matrix<T>> backwards;
			backwards.push_back(m);

			for (auto w = weights.begin(), b = biases.begin(); w < weights.end(); w++, b++)
				backwards.push_back(activation((*w ^ m) + *b));

			return backwards;
		}


		void update(Matrix<T>& data, Matrix<T>& target, const T learning_rate) {

			std::vector<Matrix<T>> values = playback(data);
			std::vector<Matrix<T>> deltas;

			Matrix<T> error_derivative = values.back() - target;
			Matrix<T> last_delta = error_derivative * activation(weights.back() ^ values.rbegin()[1]); 

			deltas.push_back(last_delta);

			for (auto value = values.rbegin() + 2, weight = weights.rbegin() + 1; value < values.rend(); value++, weight++) 
				deltas.push_back((weight + 1)->transpose() ^ deltas.back() * activation(*weight ^ *value));

			for (unsigned int i = 0; i < deltas.size(); i++) 
				weights[i] -= (deltas[deltas.size() - i - 1] ^ values[i].transpose()) * learning_rate;
		}


		void train(Matrix<T>& data, Matrix<T>& targets, const T learning_rate) {
			for (int i = 0; i < 1000; i++) 
				update(data, targets, learning_rate);
		}
};

#endif
