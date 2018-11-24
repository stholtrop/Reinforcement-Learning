#ifndef NEURALNETWORK
#define NEURALNETWORK
#include "matrix.cpp"
#include <vector>
#include <cmath>
#include "approx.cpp"

enum Activation {
	TANH, SIGMOID, LEAKYRELU, ELU
};

template <typename T>
class Activator {
public:

	static Matrix<T> activation(Matrix<T> m, Activation type) {
		auto func = [&type](T x){
			return activation(x, type);
		};
		return m.apply(func);
	}

	static Matrix<T> activationDerivative(Matrix<T> m, Activation type) {
		auto func = [&type](T y){
			return activationDerivative(y, type);
		};
		return m.apply(func);
	}

	static T activation(T x, Activation type) {
		switch (type){
			case TANH: return tanH(x);
			case SIGMOID: return sigmoid(x);
			case ELU: return elu(x);
			case LEAKYRELU: return leakyRELU(x);
			default: return sigmoid(x);
		};
	}

	static T activationDerivative(T x, Activation type) {
		switch (type) {
			case TANH: return tanHPrime(x);
			case SIGMOID: return sigmoidPrime(x);
			case ELU: return eluPrime(x);
			case LEAKYRELU: return leakyRELUPrime(x);
			default: return sigmoidPrime(x);
		}
	}

	static inline T sigmoid(T x) {
		return 1 / (1 + std::exp(-x));
	}

	static inline T sigmoidPrime(T x) {
		return sigmoid(x)*(1-sigmoid(x));
	}

	static inline T tanH(T x) {
		return 2 * sigmoid(2*x) - 1;
	}

	static inline T tanHPrime(T x) {
		return 1 - tanH(x) * tanH(x);
	}

	static inline T elu(T x, T a) {
		if (x > 0) {
			return x;
		} else {
			return a * (std::exp(x) - 1);
		};
	}

	static inline T eluPrime(T x, T a) {
		if (x > 0) {
			return 1.0;
		} else {
			return elu(x, a) + a;
		};
	}

	static inline T leakyRELU(T x) {
		if (x > 0) {
			return x;
		} else {
			return 0.01 * x;
		};
	}

	static inline T leakyRELUPrime(T x) {
		if (x > 0){
			return 1.0;
		} else {
			return 0.01;
		};
	}
};



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
			std::vector<Matrix<T>> per_layer_calculated = playback();
			Matrix<T> error_derivative = *per_layer_calculated.rbegin() - target;
		}
};

#endif
