#ifndef ACTIVATORS
#define ACTIVATORS
#include <functional>

enum Activation {
	TANH, SIGMOID, LEAKYRELU, ELU
};

template <typename T>
class Activator {
public:

	static Matrix<T> activation(const Matrix<T> &m, const Activation &type) {
		return m.apply(activation(type));
	}

	static Matrix<T> activationDerivative(const Matrix<T> &m, const Activation &type) {
		return m.apply(activationDerivative(type));
	}

	static std::function<T(T)> activation(const Activation &type) {
		switch (type){
			case TANH:
				return tanH;
			case SIGMOID:
				return sigmoid;
			case LEAKYRELU:
				return leakyRELU;
			case ELU:
				return elu;
			default:
				return sigmoid;
		};
	}

	static std::function<T(T)> activationDerivative(const Activation &type) {
		switch (type) {
			case TANH:
				return tanHPrime;
			case SIGMOID:
				return sigmoidPrime;
			case LEAKYRELU:
				return leakyRELUPrime;
			case ELU:
				return eluPrime;
			default:
				return sigmoidPrime;
		}
	}

	static inline T sigmoid(const T &x) {
		return 1 / (1 + std::exp(-x));
	}

	static inline T sigmoidPrime(const T &x) {
		return sigmoid(x)*(1-sigmoid(x));
	}

	static inline T tanH(const T &x) {
		return 2 * sigmoid(2*x) - 1;
	}

	static inline T tanHPrime(const T &x) {
		return 1 - tanH(x) * tanH(x);
	}

	static inline T elu(const T &x) {
		const T a = 0.1;
		if (x > 0) {
			return x;
		} else {
			return a * (std::exp(x) - 1);
		};
	}

	static inline T eluPrime(const T &x) {
		const T a = 0.1;
		if (x > 0) {
			return 1.0;
		} else {
			return elu(x, a) + a;
		};
	}

	static inline T leakyRELU(const T &x) {
		if (x > 0) {
			return x;
		} else {
			return 0.01 * x;
		};
	}

	static inline T leakyRELUPrime(const T &x) {
		if (x > 0){
			return 1.0;
		} else {
			return 0.01;
		};
	}
};
#endif
