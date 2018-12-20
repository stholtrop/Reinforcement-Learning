#include <iostream>
#include "qlearner.cpp"
#include "neural-network.cpp"
#include "flippo.cpp"
#include "activators.cpp"

using namespace std;

int main() {
  NeuralNetwork<double> nn = NeuralNetwork<double>({1,1,1}, new Sigmoid<double>());
  QLearner ql(nn, 0.9);
  std::vector<GameState> g = Flippo::createGame();
}
