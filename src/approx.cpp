#ifndef APPROXIMATOR
#define APPROXIMATOR
#include "matrix.cpp"

// pure virtual class

template<typename T>
class Approximator {
	virtual Matrix<T> evaluate(Matrix<T> data);
	virtual void update(const std::vector<Matrix<T>>& data, const std::vector<Matrix<T>>& target, const T learning_rate);
};

#endif
