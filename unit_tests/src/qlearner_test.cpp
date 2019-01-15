#include <iostream>
#include "qlearner.cpp"
#include "neural-network.cpp"
#include "flippo.cpp"
#include "activators.cpp"

using namespace std;



int main() {
	Function<double> *s = new ELU<double>(1), *l = new Linear<double>();
	NeuralNetwork<double> nn = NeuralNetwork<double>({64, 16, 1}, s, l);
	QLearner ql(&nn, 1);
	auto [score, wins, loses] = Flippo::randomBenchmarker(500);
	std::cout << "Initial score: " << score << " Initial wins: " << wins << " Initial loses: " << loses << " Initial draws: " << (100 - loses - wins) << endl;
	ql.replayTrain(5000, 100, 0.01, 1, 0.999, 1, -1, false, true, 100, 1000);
	delete s; delete l;
}
