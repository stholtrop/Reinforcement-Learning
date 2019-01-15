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
	cout << eta << endl;
	signal(SIGINT, gracefulExit);
	Function<double> *s = new LeakyRELU<double>(), *l = new TanH<double>();
	NeuralNetwork<double> nn = NeuralNetwork<double>({64, 48, 32, 16, 1}, s, l);
	ql = new QLearner(&nn, 1);
	auto [score, wins, loses] = Flippo::randomBenchmarker(5000);
	std::cout << "Initial score: " << score << " Initial wins: " << wins << " Initial loses: " << loses << " Initial draws: " << (100 - loses - wins) << endl;
	ql->replayTrain(5000, 100, eta, 1, 0.999, 1, -1, false, true, 10, 1000);
	auto [score1, wins1, loses1] = Flippo::randomBenchmarker(5000);
	std::cout << "Initial score: " << score1 << " Initial wins: " << wins1 << " Initial loses: " << loses1 << " Initial draws: " << (100 - loses1 - wins1) << endl;
	delete s; delete l; delete ql;
}
