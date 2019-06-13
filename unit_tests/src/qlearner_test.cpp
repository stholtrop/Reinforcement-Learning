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
	double eta = 0.01;
	signal(SIGINT, gracefulExit);
	Function<double> *s = new LeakyRELU<double>(), *l = new TanH<double>();
	NeuralNetwork<double> nn = NeuralNetwork<double>({64, 64, 32, 16, 1}, s, l);
	Flippo::initialise(&nn);
	ql = new QLearner(&nn, 1);
	auto [score, wins, loses] = Flippo::randomBenchmarker(1000);
	std::cout << "Initial score: " << score << " Initial wins: " << wins << " Initial loses: " << loses << " Initial draws: " << (100 - loses - wins) << endl;
	ql->learningTD(1, 10, eta, 0.5, 1, 1, -1, false, true, 10, 100);
	delete s; delete l; delete ql;
}
