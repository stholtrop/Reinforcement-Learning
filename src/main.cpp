#include <iostream>
#include <cmath>
#include "matrix.cpp"
#include "neural-network.cpp"
#include "activators.cpp" 


using namespace std;

int main(){
  Matrix<float> m1 = Matrix<float>(2,1, {1,0});
  Matrix<float> m2 = Matrix<float>(1,1, {0});
  Matrix<float> m3 = Matrix<float>(2,1, {0,0});
  Matrix<float> m4 = Matrix<float>(1,1, {1});
  NeuralNetwork<float> nn({2, 3, 3, 1}, new Sigmoid<float>());
  auto m5 = nn.evaluate(m1);
  nn.train(m1, m2, 0.1);
  cout << m5 << endl;
  cout << nn.evaluate(m1) << endl;
  return 0;
}
