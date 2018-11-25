#ifndef NEURALNETWORK
#define NEURALNETWORK
#include "matrix.cpp"
#include <vector>
#include <algorithm>
#include <cmath>
#include "approx.cpp"
#include "activators.cpp"
#include <functional>
#include <utility>

template<typename T>
class NeuralNetwork : public Approximator<T> {
	using VectorMatrix = typename std::vector<Matrix<T>>;
	private:

		VectorMatrix weights;
		VectorMatrix biases;

		size_t inputSize;
		size_t outputSize;

		std::function<Matrix<T>(const Matrix<T>&)> activation;
		std::function<Matrix<T>(const Matrix<T>&)> derivative;

	public:

		NeuralNetwork(std::vector<size_t> sizes, const Function<T>* av) {

			activation = Matrix<T>::wrap([av] (const T x) { return av->function(x);});
			derivative = Matrix<T>::wrap([av] (const T x) { return av->derivative(x);});
			inputSize = sizes[0];
			outputSize = *sizes.rbegin();

			for (auto p = sizes.begin(), c = sizes.begin() + 1; c < sizes.end(); c++, p++) {
				biases.push_back(Matrix<T>::initializeRandom(*c, 1));
				weights.push_back(Matrix<T>::initializeRandom(*c, *p));
			}
		}


		Matrix<T> evaluate(Matrix<T> m){

			for (auto w = weights.begin(), b = biases.begin(); w < weights.end(); w++, b++)
				m = activation((*w ^ m) + *b);

			return m;
		}


		VectorMatrix playback(const Matrix<T> m) {

			VectorMatrix backwards;
			backwards.push_back(m);

			for (auto w = weights.begin(), b = biases.begin(); w < weights.end(); w++, b++)
				backwards.push_back(activation((*w ^ m) + *b));

			return backwards;
		}

		VectorMatrix retrieve_deltas(const Matrix<T> &x, const Matrix<T> &y, const VectorMatrix &playback_data) {

			Matrix<T> error_derivative = playback_data.back() - y;
			Matrix<T> last_delta = error_derivative * derivative(weights.back() ^ playback_data.rbegin()[1]);
			VectorMatrix deltas;

			deltas.push_back(last_delta);

			for (int i = weights.size() - 2; i >= 0; i--) {

				Matrix<T> delta = weights[i+1].transpose() ^ deltas.back() * derivative(weights[i] ^ playback_data[i]);
				deltas.push_back(delta);
			}

			std::reverse(deltas.begin(), deltas.end());
			return deltas;
		}

		std::pair<VectorMatrix, VectorMatrix> retrieve_nablas(const VectorMatrix &batch_in, const VectorMatrix &batch_out, const T learning_rate) {
			// Learning rate per datapoint
			T learning_rate_real = learning_rate / ((T) batch_in.size());

			VectorMatrix nabla_b;
			VectorMatrix nabla_w;

			// Populate nablas
			for (size_t i = 0; i < weights.size(); i++) {
				nabla_b.push_back(Matrix<T>(biases[i].rows, biases[i].columns));
				nabla_w.push_back(Matrix<T>(weights[i].rows, weights[i].columns));
			}
			// Retrieve deltas per datapoint and add them up in nablas
			for (size_t i = 0; i < batch_in.size(); i++) {

				VectorMatrix playback_data = playback(batch_in[i]);
				VectorMatrix deltas = retrieve_deltas(batch_in[i], batch_out[i], playback_data);

				for (size_t j = 0; j < deltas.size(); j++) {
						nabla_w[j] += (deltas[j] ^ playback_data[i].transpose()) * learning_rate_real;
						nabla_b[j] += deltas[i];
				}
			}

			return std::make_pair(nabla_w, nabla_b);
		}


		void update(const VectorMatrix& data, const VectorMatrix& target, const T learning_rate) {
			auto nablas = retrieve_nablas(data, target, learning_rate);
			VectorMatrix nabla_w = nablas.first;
			VectorMatrix nabla_b = nablas.second;

			// Update weights
			for (size_t i = 0; i < weights.size(); i++) {
				weights[i] -= nabla_w[i];
				biases[i] -= nabla_b[i];
			}
		}

		void train(const VectorMatrix &data, const VectorMatrix &target, const T learning_rate){
			for (int i = 0; i < 1000000; i++) {
				update(data, target, learning_rate);
			}
		}
};

#endif
