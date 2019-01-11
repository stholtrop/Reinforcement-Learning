#include <iostream>
#include "qlearner.cpp"
#include "neural-network.cpp"
#include "flippo.cpp"
#include "activators.cpp"

using namespace std;



int main() {
	Function<double> *s = new Sigmoid<double>(), *l = new Linear<double>();
	NeuralNetwork<double> nn = NeuralNetwork<double>({64, 32, 1}, s, l);
	QLearner ql(&nn, 0.9);
	auto [score, wins] = Flippo::randomBenchmarker(5000);
	std::cout << "Initial score: " << score << " Initial wins: " << wins << endl;
	ql.initialize(5000, 25, 0.1, true);
	ql.replayTrain(500, 10, 0.1, 1, 0.999, 2, -1, true, true);
	delete s; delete l;
}
