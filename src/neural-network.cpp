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
#include <random>


template<typename T>
class NeuralNetwork {
	using VectorMatrix = typename std::vector<Matrix<T>>;
	private:

		VectorMatrix weights;
		VectorMatrix biases;

		size_t inputSize;
		size_t outputSize;

		std::vector<std::function<Matrix<T>(const Matrix<T>&)>> activations;
		std::vector<std::function<Matrix<T>(const Matrix<T>&)>> derivatives;

		T minimalValue;

	public:
		NeuralNetwork() {}
		
		NeuralNetwork(std::vector<size_t> sizes, const Function<T>* av, const Function<T>* f) {
			
			for (unsigned int i = 0; i < sizes.size() - 2; i++) {
				activations.push_back(Matrix<T>::wrap([av] (const T x) { return av->function(x);}));
				derivatives.push_back(Matrix<T>::wrap([av] (const T x) { return av->derivative(x);}));
			}

			activations.push_back(Matrix<T>::wrap([f] (const T x) { return f->function(x);}));
			derivatives.push_back(Matrix<T>::wrap([f] (const T x) { return f->derivative(x);}));

			minimalValue = av->min;

			inputSize = sizes[0];
			outputSize = *sizes.rbegin();

			for (auto p = sizes.begin(), c = sizes.begin() + 1; c < sizes.end(); c++, p++) {
				biases.push_back(Matrix<T>::initializeRandom(*c, 1));
				weights.push_back(Matrix<T>::initializeRandom(*c, *p));
			}
		}


		Matrix<T> evaluate(Matrix<T> m){

			for (unsigned int i = 0; i < weights.size(); i++)
				m = activations[i]((weights[i] ^ m) + biases[i]);

			return m;
		}

		void train(const VectorMatrix& data, const VectorMatrix& target, int epochs, int batchSize, T eta, bool verbose = false) {

			std::vector<int> indices(data.size());
			std::iota(indices.begin(), indices.end(), 0);
			std::default_random_engine generator(time(0));

			for (int i = 0; i < epochs; i++) {

				if (verbose) {
					std::cout << "At epoch " << i+1 << " of " << epochs << std::endl;
				}

				std::shuffle(indices.begin(), indices.end(), generator);

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

			VectorMatrix activated, results;
			activated.reserve(weights.size() + 1);
			activated.push_back(data);
			results.reserve(weights.size());

			for (unsigned int i = 0; i < biases.size(); i++) {
				results.push_back((weights[i] ^ activated.back()) + biases[i]);
				activated.push_back(activations[i](results.back()));
			}

			Matrix<T> delta = (activated.back() - target) * (derivatives.back())(results.back());
			nabla_b.back() += delta;
			nabla_w.back() += delta ^ activated[activated.size() - 2].transpose();

			for (int i = results.size() - 2; i >= 0; i--) {
				delta = (weights[i + 1].transpose() ^ delta) * derivatives[i](results[i]);
				nabla_b[i] += delta;
				nabla_w[i] += delta ^ activated[i].transpose();
			}
		}

		Matrix<T> min() {
			std::vector<T> v(outputSize);

			for (unsigned int i = 0; i < v.size(); i++)
				v[i] = minimalValue;

			return Matrix<T>(1, outputSize, v);
		}
};

#endif
