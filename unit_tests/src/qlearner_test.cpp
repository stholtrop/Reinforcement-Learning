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
	ql.initialize(5000, 100, 0.005, true);
	auto [score3, wins3] = Flippo::randomBenchmarker(1000);
	cout << "Avg score: " << score3 << " Percentage won: " << wins3 << endl;
	ql.initialize(5000, 100, 0.003, true);
	auto [score4, wins4] = Flippo::randomBenchmarker(1000);
	cout << "Avg score: " << score4 << " Percentage won: " << wins4 << endl;
	ql.initialize(5000, 100, 0.001, true);
	nn.printNetwork();
	auto [score, wins] = Flippo::randomBenchmarker(5000);
	cout << "Avg score: " << score << " Percentage won: " << wins << endl;
	delete s; delete l;
}
