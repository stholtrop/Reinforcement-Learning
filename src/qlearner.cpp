#ifndef QLEARNER
#define QLEARNER
#include "neural-network.cpp"
#include "flippo.cpp"

class QLearner {
	using VectorMatrix = std::vector<Matrix<double>>;
	NeuralNetwork<double> approximator;
	double gamma;
	public:

		QLearner(NeuralNetwork<double>& a, double g) : approximator(a), gamma(g) {}

		void initialize(int games, bool verbose = false) {

		}

		void replayTrain(bool verbose = false) {

		}

		void train(VectorMatrix &target, VectorMatrix &data) {
			
		}


};
#endif
