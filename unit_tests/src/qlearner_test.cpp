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
	auto [score, wins, loses] = Flippo::randomBenchmarker(5000);
	std::cout << "Initial score: " << score << " Initial wins: " << wins << " Initial loses: " << loses << " Initial draws: " << (100 - loses - wins) << endl;
	ql.replayTrain(300, 50, 0.1, 0.5, 0.999, 5, -1, true, true, 10, 100);
	delete s; delete l;
}
