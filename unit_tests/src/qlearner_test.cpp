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
	double eta = 0.001;
	signal(SIGINT, gracefulExit);
	Function<double> *s = new LeakyRELU<double>(), *l = new TanH<double>();
	Regularizer<double> *r = new ExponentialDecayRegularizer<double>(-0.1);
	NeuralNetwork<double> nn = NeuralNetwork<double>({64, 48, 32, 32, 32, 32, 16, 1}, s, l, r);
	Flippo::initialise(&nn);
	ql = new QLearner(&nn, 1);
	ql->learningTD(500, 100, eta, 0.1, 1, 1, -1, false, true, 10, 500);
	auto [score1, wins1, loses1] = Flippo::randomBenchmarker(5000);
	std::cout << "Initial score: " << score1 << " Initial wins: " << wins1 << " Initial loses: " << loses1 << " Initial draws: " << (100 - loses1 - wins1) << endl;
	delete s; delete l; delete ql; delete r;
}
