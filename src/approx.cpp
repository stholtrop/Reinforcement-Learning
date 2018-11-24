#ifndef APPROXIMATOR
#define APPROXIMATOR
#include "matrix.cpp"

// pure virtual class

template<typename T>
class Approximator {
	virtual Matrix<T> evaluate(Matrix<T> data); 
	virtual void update(Matrix<T>& data, Matrix<T>& target);
};

#endif
