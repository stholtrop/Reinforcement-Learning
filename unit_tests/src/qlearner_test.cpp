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
	Function<double> *s = new LeakyRELU<double>(), *l = new TanH<double>();
	NeuralNetwork<double> nn = NeuralNetwork<double>({64, 32, 1}, s, l);
	ql = new QLearner(&nn, 1);
	auto [score, wins, loses] = Flippo::randomBenchmarker(10);
	std::cout << "Initial score: " << score << " Initial wins: " << wins << " Initial loses: " << loses << " Initial draws: " << (100 - loses - wins) << endl;
	ql->replayTrain(1000, 100, 0.001, 1, 0.999, 1, -1, true, true, 10, 1000);
	delete s; delete l; delete ql;
}
