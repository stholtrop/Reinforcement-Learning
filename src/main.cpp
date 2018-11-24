#include <iostream>
#include <cmath>
#include "matrix.cpp"
#include "neural-network.cpp"
#include "activators.cpp" 


using namespace std;

int main(){
  Matrix<float> m1 = Matrix<float>(2,1, {1,1});
  Matrix<float> m2 = Matrix<float>(1,1, {0});
  NeuralNetwork<float> nn({2, 3, 3, 1}, Activation::SIGMOID);
  auto m3 = nn.evaluate(m1);
  cout << m3 << endl;
  nn.train(m1, m2, 0.1);
  cout << m3 << endl;
  cout << nn.evaluate(m1) << endl;
}
