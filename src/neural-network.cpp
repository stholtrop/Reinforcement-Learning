#ifndef NEURALNETWORK
#define NEURALNETWORK
#include "matrix.cpp"
#include <vector>
#include <algorithm>
#include <cmath>
#include "activators.cpp"
#include <functional>
#include <utility>
#include <numeric>


template<typename T>
class NeuralNetwork {
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

		void train(const VectorMatrix& data, const VectorMatrix& target, int epochs, int batchSize, T eta) {
			std::vector<int> indices(data.size());
			std::iota(indices.begin(), indices.end(), 0);
			for (int i = 0; i < epochs; i++) {
				std::random_shuffle(indices.begin(), indices.end());
				for (auto j = indices.begin(); j < indices.end(); j += batchSize) {
					int size = (j + batchSize > indices.end()) ? indices.end() - j: batchSize;
					VectorMatrix batch_data(size);
					VectorMatrix batch_target(size);
					std::generate(batch_data.begin(), batch_data.end(), [&, n = 0] () mutable {
						return data[*(j + n++)];
					});
					std::generate(batch_target.begin(), batch_target.end(), [&, n = 0] () mutable {
						return target[*(j + n++)];
					});
					updateBatch(batch_data, batch_target, eta);
				}
				std::cout << i << std::endl;
			}
		}

		void updateBatch(const VectorMatrix& data, const VectorMatrix& target, T eta) {

			VectorMatrix nabla_b, nabla_w;
			nabla_b.reserve(biases.size());
			nabla_w.reserve(weights.size());

			for (unsigned int i = 0; i < biases.size(); i++) {
				nabla_b.push_back(Matrix<T>(biases[i].rows, biases[i].columns));
				nabla_w.push_back(Matrix<T>(weights[i].rows, weights[i].columns));
			}

			for (unsigned int i = 0; i < data.size(); i++) {
				updateNablas(data[i], target[i], nabla_b, nabla_w);
			}

			for (unsigned int i = 0; i < biases.size(); i++) {
				weights[i] -= nabla_w[i] * (eta/data.size());
				biases[i] -= nabla_b[i] * (eta/data.size());
			}
		}

		void updateNablas(const Matrix<T>& data, const Matrix<T>& target, VectorMatrix& nabla_b, VectorMatrix& nabla_w) {

			VectorMatrix activations, results;
			activations.reserve(weights.size() + 1);
			activations.push_back(data);
			results.reserve(weights.size());

			for (unsigned int i = 0; i < biases.size(); i++) {
				results.push_back((weights[i] ^ activations.back()) + biases[i]);
				activations.push_back(activation(results.back()));
			}

			Matrix<T> delta = (activations.back() - target) * derivative(results.back());
			nabla_b.back() += delta;
			nabla_w.back() += delta ^ activations[activations.size() - 2].transpose();

			for (int i = results.size() - 2; i >= 0; i--) {
				delta = (weights[i + 1].transpose() ^ delta) * derivative(results[i]);
				nabla_b[i] += delta;
				nabla_w[i] += delta ^ activations[i].transpose();
			}
		}
};

#endif
