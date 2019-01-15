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
#include <fstream>
#include <string>


template<typename T>
class NeuralNetwork {
	using VectorMatrix = typename std::vector<Matrix<T>>;
	private:

		VectorMatrix weights;
		VectorMatrix biases;

		size_t inputSize;
		size_t outputSize;

		const Function<T>* activationFunction;
		const Function<T>* finalFunction;
		constexpr static T threshold = 100;

		std::vector<std::function<Matrix<T>(const Matrix<T>&)>> activations;
		std::vector<std::function<Matrix<T>(const Matrix<T>&)>> derivatives;

	public:
		NeuralNetwork() {}

		NeuralNetwork(const std::string &filename) {
			std::cout << "Initializing from filename" << std::endl;
			readNetwork(filename);
		}

		NeuralNetwork(std::vector<size_t> sizes, const Function<T>* av, const Function<T>* f) {

			activationFunction = av;
			finalFunction = f;

			for (unsigned int i = 0; i < sizes.size() - 2; i++) {
				activations.push_back(Matrix<T>::wrap([av] (const T x) { return av->function(x);}));
				derivatives.push_back(Matrix<T>::wrap([av] (const T x) { return av->derivative(x);}));
			}

			activations.push_back(Matrix<T>::wrap([f] (const T x) { return f->function(x);}));
			derivatives.push_back(Matrix<T>::wrap([f] (const T x) { return f->derivative(x);}));

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
				bool reduction = reduceToThresholdOrVoidNAN();
				if (reduction) {
					std::cout << "Reduction was necessary" << std::endl;
				}
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

	void saveNetwork(const std::string &filename) const {
		/*
		* Save file layout:
		* Magic: SSVN
		* inputSize, outputSize
		* numLayers
		* weights, biases
		* avID, fID
		* File extension: .ssvn
		*/
		std::ofstream file;
		try {
			file.open(filename);
		} catch (const std::ofstream::failure &e) {
			std::cerr << "Error reading file" << std::endl;
			throw "Error writing file";
		}

		file << "SSVN ";
		file << inputSize << " " << outputSize << " ";

		file << weights.size() << " ";
		for (auto w : weights) {
			w.writeToFile(file);
		}

		for (auto b : biases) {
			b.writeToFile(file);
		}

		file << activationFunction->getID() << " " << finalFunction->getID();
		file.close();
	}

	void readNetwork(const std::string &filename) {
		std::ifstream file;
		try {
			file.open(filename);
		} catch (const std::ifstream::failure &e) {
			std::cerr << "Error reading file" << std::endl;
			throw "Error reading file";
		}

		// Ignore magic SSVN
		file.ignore(4, ' ');

		// Initialize weights and biases
		int numLayers;
		file >> inputSize >> outputSize >> numLayers;
		for (int i = 0; i < numLayers; i++) {
			Matrix<T> temp;
			temp = Matrix<T>::readFromFile(file);
			weights.push_back(temp);
		}

		// Get sizes from biases
		std::vector<size_t> sizes;
		sizes.push_back(inputSize);
		for (int j = 0; j < numLayers; j++) {
			Matrix<T> temp;
			temp = Matrix<T>::readFromFile(file);
			biases.push_back(temp);
			sizes.push_back(temp.rows);
		}

		// Initialize activations and derivatives
		int aID, fID;
		file >> aID >> fID;
		Function<T>* av = getFunctionFromID(aID);
		Function<T>* f  = getFunctionFromID(fID);
		activationFunction = av;
		finalFunction = f;

		for (unsigned int i = 0; i < sizes.size() - 2; i++) {
			activations.push_back(Matrix<T>::wrap([av] (const T x) { return av->function(x);}));
			derivatives.push_back(Matrix<T>::wrap([av] (const T x) { return av->derivative(x);}));
		}
		activations.push_back(Matrix<T>::wrap([f] (const T x) { return f->function(x);}));
		derivatives.push_back(Matrix<T>::wrap([f] (const T x) { return f->derivative(x);}));

		file.close();
	}

	static Function<T>* getFunctionFromID(const int ID, const T extra = 0) {
		// This function has to be put here, as the templates can now be matched
		switch (ID) {
			case 1: return new Linear<T>();
			case 2: return new Sigmoid<T>();
			case 3: return new TanH<T>();
			case 4: return new RELU<T>();
			case 5: return new LeakyRELU<T>();
			case 6: return new ELU<T>(extra);
			default: return new Linear<T>();
		}
	}

	void printNetwork() {

		for (unsigned int i = 0; i < weights.size(); i++) {
			std::cout << weights[i] << std::endl << biases[i] << std::endl;
		}

	}

	bool reduceToThresholdOrVoidNAN() {
		bool detected = false;
		for (unsigned int i = 0; i < biases.size(); i++) {
			for (unsigned int j = 0; j < biases[i].rows * biases[i].columns; j++) {
				if (biases[i][j] > threshold || biases[i][j] < -threshold || std::isnan(biases[i][j])) {
					if (std::signbit(biases[i][j])) {
						biases[i][j] = 1-threshold;
					}else {
						biases[i][j] = threshold-1;
					}
					detected = true;
				}
			}
		}
		for (unsigned int i = 0; i < weights.size(); i++) {
			for (unsigned int j = 0; j < weights[i].rows * weights[i].columns; j++) {
				if (weights[i][j] > threshold || weights[i][j] < -threshold || std::isnan(weights[i][j])) {
					if (std::signbit(weights[i][j])) {
						weights[i][j] = 1-threshold;
					}else {
						weights[i][j] = threshold-1;
					}
					detected = true;
				}
			}
		}
		return detected;
	}

	bool containsNan() {
		return std::isnan(weights[0][0]);
	}
};

#endif
