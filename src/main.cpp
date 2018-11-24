#include <iostream>
#include <cmath>
#include "matrix.cpp"
#include "neural-network.cpp"


using namespace std;

int main(){
  Matrix<float> m1 = Matrix<float>(2, 2, {1,2,3,4});
  Matrix<float> m2 = Activator<float>::activation(m1, Activation::SIGMOID);
  cout << m1 << endl;
  cout << m2 << endl;
  NeuralNetwork<float> nn({2, 6, 1}, Activation::SIGMOID);
  auto m3 = nn.evaluate(m1);
  cout << m3 << endl;
}
