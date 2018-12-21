#ifndef QLEARNER
#define QLEARNER
#include "neural-network.cpp"
#include "flippo.cpp"

class QLearner {

	using VectorMatrix = std::vector<Matrix<double>>;
    using VectorGameState = std::vector<GameState>;
	NeuralNetwork<double>* approximator;
	double gamma;

	public:

		QLearner(NeuralNetwork<double>* a, double g) : approximator(a), gamma(g) {
			Flippo::nn = approximator;	
		}

		void initialize(int nGames, int batchSize, double eta) {

			std::vector<VectorGameState> games(nGames);
			std::generate(games.begin(), games.end(), Flippo::createGame);

			for (unsigned int i = 0; i < games[0].size(); i++) {

				VectorMatrix data, target;
				data.reserve(nGames * (i + 1));
				target.reserve(nGames * (i + 1));

				for (int j = games[0].size() - i - 1; j >= 0; j--) {
					for (int k = 0; k < nGames; k++) {

						data.push_back(games[k][j].input());
						target.push_back(Flippo::getTarget(games[k][j]));

					}
				}

				approximator->train(data, target, 1, batchSize, eta);

			}

		}

		void replayTrain(bool verbose = false) {

		}
};
#endif
