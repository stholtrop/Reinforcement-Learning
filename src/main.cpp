#include <iostream>
#include <cmath>
#include "matrix.cpp"
#include "neural-network.cpp"
#include "activators.cpp"


using namespace std;

int main(){
  Matrix<float> m1 = Matrix<float>(2,1, {1,0});
  Matrix<float> m2 = Matrix<float>(1,1, {0});
  Matrix<float> m3 = Matrix<float>(2,1, {0,1});
  Matrix<float> m4 = Matrix<float>(1,1, {1});
  NeuralNetwork<float> nn({2, 3, 1}, new Sigmoid<float>());
  cout << nn.evaluate(m1) << endl;
  cout << nn.evaluate(m3) << endl;
  nn.train({m1, m3}, {m2, m4}, 1000);
  cout << nn.evaluate(m1) << endl;
  cout << nn.evaluate(m3) << endl;
  return 0;
}
