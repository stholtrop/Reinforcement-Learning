#ifndef NEURALNETWORK
#define NEURALNETWORK
#include "matrix.cpp"
#include <vector>
#include <cmath>
#include "approx.cpp"

enum Activation {
  TANH, SIGMOID
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
      default: return sigmoid(x);
    };
  }

  static T activationDerivative(T y, Activation type) {
    switch (type) {
      case TANH: return tanHPrime(y);
      case SIGMOID: return sigmoidPrime(y);
      default: return sigmoidPrime(y);
    }
  }

  static inline T sigmoid(T x) {
    return 1 / (1 + std::exp(-x));
  }

  static inline T sigmoidPrime(T y) {
    return y*(1-y);
  }

  static inline T tanH(T x) {
    return 2 * sigmoid(2*x) - 1;
  }

  static inline T tanHPrime(T y) {
    return 1 / (1 - y * y);
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
        std::cout << m << std::endl;
        std::cout << "before: "<< m.rows << "x" << m.columns << std::endl;
        m = (*w ^ m) + *b
        std::cout << m.rows << "x" << m.columns << std::endl;
      }
			return m;
		}

		void update(Matrix<T>& data, Matrix<T>& target) {

		}
};

#endif
