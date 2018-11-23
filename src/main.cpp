#include <iostream>
#include <cmath>
#include "matrix.cpp"
#include "neural-network.cpp"


using namespace std;

int main(){
  Matrix<float> m1 = Matrix<float>::initializeRandom(2, 1);
  NeuralNetwork<float> nn({2, 6, 1});
  auto m2 = nn.evaluate(m1);
  cout << m2 << endl;
}
