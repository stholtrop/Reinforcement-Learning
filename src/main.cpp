#include <iostream>
#include <cmath>
#include "matrix.cpp"
#include "neural-network.cpp"


using namespace std;

int main(){
  Matrix<double> m1 = Matrix<double>::initializeRandom(2,2);
  cout << m1;
  NeuralNetwork nn({1,2});
}
