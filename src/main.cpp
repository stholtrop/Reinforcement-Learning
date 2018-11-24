#include <iostream>
#include <cmath>
#include "matrix.cpp"
#include "neural-network.cpp"
#include "activators.cpp" 


using namespace std;

int main(){
  Matrix<float> m1 = Matrix<float>(2,1, {1,2});
  NeuralNetwork<float> nn({2, 6, 1}, Sigmoid<float>());
  nn.playback(m1);
}
