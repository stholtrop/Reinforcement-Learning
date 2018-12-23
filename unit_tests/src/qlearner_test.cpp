#include <iostream>
#include "qlearner.cpp"
#include "neural-network.cpp"
#include "flippo.cpp"
#include "activators.cpp"

using namespace std;



int main() {
  NeuralNetwork<double> nn = NeuralNetwork<double>({64,32,1}, new Sigmoid<double>(), new Linear<double>());
  QLearner ql(&nn, 0.9);
  auto [score2, wins2] = Flippo::randomBenchmarker(5000);
  cout << "Avg score: " << score2 << " Percentage won: " << wins2 << endl;
  ql.initialize(100, 10, 0.1, true);
  auto [score, wins] = Flippo::randomBenchmarker(5000);
  cout << "Avg score: " << score << " Percentage won: " << wins << endl;
}
