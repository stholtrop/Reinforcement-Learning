#include <iostream>
#include "qlearner.cpp"
#include "neural-network.cpp"
#include "flippo.cpp"
#include "activators.cpp"
#include <csignal>

using namespace std;

QLearner *ql;

void gracefulExit(int signum) {
  cout << endl << "Shutting down, printing network: " << endl;
	ql->approximator->printNetwork();
	cout << flush;
  exit(signum);
}

int main() {
	signal(SIGINT, gracefulExit);
	Function<double> *s = new ELU<double>(1), *l = new Linear<double>();
	NeuralNetwork<double> nn = NeuralNetwork<double>({64, 32, 1}, s, l);
	ql = new QLearner(&nn, 1);
	auto [score, wins, loses] = Flippo::randomBenchmarker(10);
	std::cout << "Initial score: " << score << " Initial wins: " << wins << " Initial loses: " << loses << " Initial draws: " << (100 - loses - wins) << endl;
	ql->replayTrain(10, 5, 0.000001, 0.5, 0.999, 5, -1, false, true, 10, 100);
	delete s; delete l; delete ql;
}
