#include <iostream>
#include <cmath>
#include "matrix.cpp"
#include "neural-network.cpp"


using namespace std;

int main(){
  Matrix<float> m1 = Matrix<float>::initializeRandom(1,2);
  NeuralNetwork nn({1, 6, 1});
  cout << "m1:\n" << m1 << endl;
  auto m2 = nn.predict(m1);
  cout << "m2:\n" << m2 << endl;
  cout << "m1:\n" << m1 << endl;
}
