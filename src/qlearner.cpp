#ifndef QLEARNER
#define QLEARNER
#include "neural-network.cpp"
#include "flippo.cpp"
#include <string>

class QLearner {

	using VectorMatrix = std::vector<Matrix<double>>;
    using VectorGameState = std::vector<GameState>;

	public:
		NeuralNetwork<double>* approximator;
		double gamma;

		QLearner(NeuralNetwork<double>* a, double g) : approximator(a), gamma(g) {}

		QLearner(const std::string &filename, double g) {
			approximator = new NeuralNetwork<double>(filename);
			gamma = g;
		}

		void learningTD(int nGames, int batchSize, double eta, double epsilon, double decay, int repeats = 1, int epochs = -1, bool verbose = false, bool test = false, int testRate = 100, int testSize=5000) {

			for (; epochs != 0; epochs--) {
				std::vector<VectorGameState> games(nGames);
				std::generate(games.begin(), games.end(), [epsilon] () {return Flippo::createGame(epsilon);});
				VectorMatrix data, target;
				size_t s = games[0].size();

				for (unsigned int i = 0; i < s; i++) {

					for (int k = 0; k < nGames; k++) {

						data.push_back(games[k][s - i - 1].input());
						target.push_back(Flippo::getTarget(games[k][s - i - 1]));

					}
				}

				approximator->train(data, target, repeats, batchSize, eta);

				if (verbose) {
					std::cout << "Loop: " << epochs << '\n';
				}

				if (test && epochs%testRate == 0) {
					auto [score, win, lose] = Flippo::randomBenchmarker(testSize);
					if (verbose) {
						approximator->printNetwork();
					}
					std::cout << "Avg score: " << score << " Percentage won: " << win <<  " Percentage lost: " << lose << " Percentage draw: " << (100 - win - lose) << std::endl;

				}

				epsilon *= decay;

			}

		}

		void learningMC(int nGames, int batchSize, double eta, double epsilon, double decay, int repeats = 1, int epochs = -1, bool verbose = false, bool test = false, int testRate = 100, int testSize=5000) {

			for (; epochs != 0; epochs--) {
				std::vector<VectorGameState> games(nGames);
				std::generate(games.begin(), games.end(), [epsilon] () {return Flippo::createGameWinner(epsilon);});
				VectorMatrix data, target;
				size_t s = games[0].size();

				for (unsigned int i = 0; i < s; i++) {

					int c = games[0][s - i - 1].getColour();

					for (int k = 0; k < nGames; k++) {

						data.push_back(c*games[k][s - i - 1].input());
						target.push_back(Matrix<double>(1, 1, {c*games[k][s - i - 1].getScore()})/64);

					}
				}

				approximator->train(data, target, repeats, batchSize, eta);

				if (verbose) {
					std::cout << "Loop: " << epochs << '\n';
				}

				if (test && epochs%testRate == 0) {
					auto [score, win, lose] = Flippo::randomBenchmarker(testSize);
					if (verbose) {
						approximator->printNetwork();
					}
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
