#ifndef QLEARNER
#define QLEARNER
#include "neural-network.cpp"
#include "flippo.cpp"
#include <string>

class QLearner {

	using VectorMatrix = std::vector<Matrix<double>>;
    using VectorGameState = std::vector<GameState>;
	NeuralNetwork<double>* approximator;
	double gamma;

	public:

		QLearner(NeuralNetwork<double>* a, double g) : approximator(a), gamma(g) {
			Flippo::initialise(approximator);
		}

		QLearner(const std::string &filename, double g) {
			approximator = new NeuralNetwork<double>(filename);
			gamma = g;
		}

		void initialize(int nGames, int batchSize, double eta, bool verbose = false) {

			std::vector<VectorGameState> games(nGames);
			std::generate(games.begin(), games.end(), Flippo::createGame);
			VectorMatrix data, target;

			for (unsigned int i = 0; i < games[0].size(); i++) {
				for (int k = 0; k < nGames; k++) {

					data.push_back(games[k][games[0].size() - i - 1].input());
					target.push_back(Flippo::getTarget(games[k][games[0].size() - 1 - i]));

				}

				if (verbose) {
					std::cout << "Training " << i+1 << " of " << games[0].size() << std::endl;
				}

				approximator->train(data, target, 1, batchSize, eta);

			}

		}

		void replayTrain(bool verbose = false) {

		}

		void save(const std::string &filename) {
			approximator->saveNetwork(filename);
		}
};
#endif
