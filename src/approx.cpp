#ifndef APPROXIMATOR
#define APPROXIMATOR
#include "matrix.cpp"

// pure virtual class

template<typename T>
class Approximator {
	virtual Matrix<T> evaluate(Matrix<T> data) = 0;
};

#endif
