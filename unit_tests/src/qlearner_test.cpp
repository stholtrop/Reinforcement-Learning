#include <iostream>
#include "qlearner.cpp"
#include "neural-network.cpp"
#include "flippo.cpp"
#include "activators.cpp"

using namespace std;



int main() {
  cout << "Start" << endl;
  Function<double> *s = new Sigmoid<double>(), *l = new Linear<double>();
  NeuralNetwork<double> nn = NeuralNetwork<double>({64, 32, 1}, s, l);
  QLearner ql(&nn, 0.9);
  cout << "Initialized qlearner" << endl;
  auto [score2, wins2] = Flippo::randomBenchmarker(5000);
  cout << "Finished benchmark" << endl;
  cout << "Avg score: " << score2 << " Percentage won: " << wins2 << endl;
  ql.initialize(5000, 50, 0.1, true);
  auto [score, wins] = Flippo::randomBenchmarker(5000);
  cout << "Avg score: " << score << " Percentage won: " << wins << endl;
  delete s; delete l;
}
