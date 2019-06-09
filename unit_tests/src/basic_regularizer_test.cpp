#include "regularizers.cpp"
#include <iostream>

int main () {
    Regularizer<float> *x = new ExponentialDecayRegularizer<float>(0.3);
    Matrix<float> t = Matrix<float>(2,2, {12,12,1,2});
    std::cout << t[0] << std::endl;
    std::cout << x->penalty(t, 1) << std::endl;
}