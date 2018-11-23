#include "matrix.cpp"

// pure virtual class

class Approximator {
	virtual Matrix<float> evaluate(Matrix<float> data); 
	virtual void update(Matrix<float> data);
}
