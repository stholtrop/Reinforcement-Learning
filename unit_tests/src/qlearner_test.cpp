#include <iostream>
#include "qlearner.cpp"
#include "neural-network.cpp"
#include "flippo.cpp"
#include "activators.cpp"

using namespace std;



int main() {
  NeuralNetwork<double> nn = NeuralNetwork<double>({64,32,1}, new Sigmoid<double>(), new Linear<double>());
  QLearner ql(&nn, 0.9);
  ql.initialize(100, 10, 3.0);
  cout << Flippo::randomBenchmarker() << endl;
}
