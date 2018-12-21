#ifndef ACTIVATORS
#define ACTIVATORS
#include <functional>
#include <cmath>

template<typename T>
class Function {
	public:
		T min = -1;

		virtual T function(T x) const = 0;
		virtual T derivative(T x) const = 0;
};

template<typename T>
class Sigmoid : public Function<T> {
	public:

		T function(T x) const {
			return 1 / (1 + std::exp(-x));
		}

		T derivative(T x) const {
			return function(x) * (1 - function(x));
		}
};

template<typename T>
class TanH : public Function<T> {
	public:

		T function(T x) const {
			return 2 / (1 + std::exp(-2*x)) - 1;
		}

		T derivative(T x) const {
			return 1 - function(x)*function(x);
		}
};

template<typename T>
class RELU : public Function<T> {
	public:

		T function(T x) const {
			return x > 0 ? x : 0;
		}

		T derivative(T x) const {
			return x > 0 ? 1 : 0;
		}
};

template<typename T>
class LeakyRELU : public Function<T> {
	public:

		T function(T x) const {
			return x > 0 ? x : 0.1 * x;			
		}

		T derivative(T x) const {
			return x > 0 ? 1 : 0.1;
		}
};

template<typename T>
class ELU : public Function<T> {
	private:

		T a;

	public:

		ELU(T x) : a(x) {}

		T function(T x) const {
			return x > 0 ? x : a * (std::exp(x) - 1);
		}

		T derivative(T x) const {
			return x > 0 ? 1 : a * std::exp(x);
		}
};

#endif
