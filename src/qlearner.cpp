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
			std::generate(games.begin(), games.end(), [] () {return Flippo::createGame(1);});
			VectorMatrix data, target;
			size_t s = games[0].size();

			for (unsigned int i = 0; i < s; i++) {

				int c = games[0][s - i - 1].getColour();

				for (int k = 0; k < nGames; k++) {

					data.push_back(c*games[k][s - i - 1].input());
					target.push_back(Flippo::getTarget(games[k][s - 1 - i]));

				}

				if (verbose) {
					std::cout << "Training " << i << " of " << s - 1 << std::endl;
				}

				approximator->train(data, target, 1, batchSize, eta);

			}

		}

		void replayTrain(int nGames, int batchSize, double eta, double epsilon, double decay, int repeats = 1, int epochs = -1, bool verbose = false, bool test = false, int testRate = 100, int testSize=5000) {
			for (; epochs != 0; epochs--) {
				std::vector<VectorGameState> games(nGames);
				std::generate(games.begin(), games.end(), [epsilon] () {return Flippo::createGame(epsilon);});
				VectorMatrix data, target;
				size_t s = games[0].size();

				for (unsigned int i = 0; i < s; i++) {
					
					int c = games[0][s - i - 1].getColour();

					for (int k = 0; k < nGames; k++) {

						data.push_back(c*games[k][s - i - 1].input());
						target.push_back(Flippo::getTarget(games[k][s - 1 - i]));

					}
				}

				approximator->train(data, target, repeats, batchSize, eta);

				if (approximator->containsNan()) {
					for (int i = 0; i < data.size(); i++) {
						std::cout << data[i].transpose() << std::endl;
						std::cout << target[i] << std::endl;
					}
					break;
				}

				if (verbose) {
					std::cout << "Loop: " << epochs << '\n';
				}

				if (test && epochs%testRate == 0) {
					auto [score, win, lose] = Flippo::randomBenchmarker(testSize);
					std::cout << "Avg score: " << score << " Percentage won: " << win <<  " Percentage lost: " << lose << " Percentage draw: " << (100 - win - lose) << std::endl;

				}
				epsilon *= decay;

			}

		}

		void save(const std::string &filename) {
			approximator->saveNetwork(filename);
		}
};
#endif
