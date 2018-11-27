#include <iostream>
#include <cmath>
#include "matrix.cpp"
#include "neural-network.cpp"
#include "activators.cpp"


using namespace std;

int main(){
  Matrix<float> m1 = Matrix<float>::readFromFile("./data/mnist_test.ssv");
  cout << m1;
}
