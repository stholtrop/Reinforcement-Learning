#ifndef REGULARIZERS
#define REGULARIZERS
#include <functional>
#include <vector>
#include "matrix.cpp"
#include <cmath>


template<typename T>
class Regularizer {
	public:
		virtual Matrix<T> penalty(const Matrix<T> w, int current) const = 0;
};

template<typename T>
class BasicRegularizer : public Regularizer<T> {
    private:
        T lambda;
    public:
        BasicRegularizer(const T la) {
            lambda = la;
        }
        inline Matrix<T> penalty(const Matrix<T> w, int current) const {
            return w*lambda;
        }
};

template<typename T>
class ExponentialDecayRegularizer : public Regularizer<T> {
    private:
        T lambda;
    public:
        ExponentialDecayRegularizer(const T la) {
            lambda = la;
        }
        inline Matrix<T> penalty(const Matrix<T> w, int current) const {
            return w*std::exp(lambda * (T)current);
        }
};

#endif

