#ifndef FLIPPO
#define FLIPPO
#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include "matrix.cpp"
#include <iostream>
#include <random>

const int BOARD_WIDTH = 8;
const int BOARD_HEIGHT = 8;
const int BOARD_SIZE = BOARD_WIDTH * BOARD_HEIGHT;


class GameState {
public:
	Matrix<double> board;
	int moves;

	GameState() {
		int middle_x = BOARD_WIDTH/2-1;
		int middle_y = BOARD_HEIGHT/2-1;
		board = Matrix<double>(BOARD_HEIGHT, BOARD_WIDTH);
		board(middle_y, middle_x) = 1;
		board(middle_y+1, middle_x + 1) = 1;
		board(middle_y, middle_x + 1) = -1;
		board(middle_y+1, middle_x) = -1;
		moves = 4;
	}

  GameState(Matrix<double> b, int m = 4) : board(b), moves(m) {}

	void placePiece(const int i, const int j, const double c) {
		board(i, j) = c;
		// Loop over directions
		for (int di = -1; di < 2; di++) {
			for (int dj = -1; dj < 2; dj++) {
				if (di == 0 && dj == 0) {
					continue;
				}
				update(i, j, di, dj, c);
			}
		}
		moves++;
	}

	double getScore() const {
		double result = 0;
		for (int i = 0; i < BOARD_SIZE; i++) {
			result += board[i];
		}
		return result;
	}

	void update(const int i, const int j, const int di, const int dj, const double c) {
		// Update along a certain direction given by di, dj
		auto [fi, fj] = getFlip(i, j, di, dj, c);
		if (fi == i && fj == j) {
			return;
		}
		fi -= di;
		fj -= dj;
		while (fi != i || fj != j) {
			board(fi, fj) *= -1.0;
			fi -= di;
			fj -= dj;
		}
	}

	std::vector<std::tuple<int, int>> getNeighbours(const int i, const int j) const {

		std::vector<std::tuple<int, int>> neighbours;

		for (int ni = i - 1; ni < i + 2; ni++)
			for (int nj = j - 1; nj < j + 2; nj++)
				if (inbound(ni, nj) && board(ni, nj) != 0)
			neighbours.push_back({ni, nj});

		return neighbours;

	}

	bool hasNeighbours(const int i, const int j) const {

	 for (int ni = i - 1; ni < i + 2; ni++)
			for (int nj = j - 1; nj < j + 2; nj++)
				if (inbound(ni, nj) && board(ni, nj) != 0)
			return true;

		return false;
	}

	inline bool inbound(const int i, const int j) const {
		return (0 <= i) && (i <	BOARD_HEIGHT) && (0 <= j) && (j < BOARD_WIDTH);
	}

	std::tuple<int, int> getFlip(int i, int j, const int di, const int dj, const double c) const {

		// Find flip of color along direction di, dj
		i += di; j += dj;
		int fi = i, fj = j;

		while (inbound(i, j)) {

			if (board(i, j) == 0) break;
			if (board(i, j) == c) {fi = i; fj = j;}
			i += di; j += dj;

		}

		return {fi, fj};

	}

	int valid(const int i, const int j, const double c, bool& flip) const {

		if (board(i, j) != 0) return 0;

		if (!hasNeighbours(i, j)) return 0;

		for (int di = -1; di < 2; di++) {

			for (int dj = -1; dj < 2; dj++) {

				if (di == 0 && dj == 0) continue;
				auto [fi, fj] = getFlip(i, j, di, dj, c);
				if ((fi != i || fj != j) && (fi != i + di || fj != j + dj)) {
          flip = true;
          return 2;
        }
			}
		}

		return 1;
	}

	std::vector<std::tuple<int, int>> validMoves(const double c) const {

		Matrix<int> move_matrix(BOARD_HEIGHT, BOARD_WIDTH);
    	std::vector<std::tuple<int, int>> indices;
   		bool flip = false;
		for (int i = 0; i < BOARD_HEIGHT; i++) {
			for (int j = 0; j < BOARD_WIDTH; j++) {
				move_matrix(i, j) = valid(i, j, c, flip);
			}
		}

		if (flip)
		  move_matrix = move_matrix.apply([] (int x) {return x >> 1;});

		for (int i = 0; i < BOARD_HEIGHT; i++)
		  for (int j = 0; j < BOARD_WIDTH; j++)
			if (move_matrix(i, j))
			  indices.push_back({i, j});

		return indices;
	}

	GameState potentialBoard(const int i, const int j, const double c) const {

		GameState copy(*this);
		copy.placePiece(i, j, c);
		return copy;

	}

	Matrix<double> input() {
		return board.reshape(BOARD_SIZE, 1);
	}

	inline bool isFinal() const {
		return moves == BOARD_SIZE;
	}


	std::string toString() const {
		return board.toString();
	}

  int emptyPlaces() {
    int result = 0;
    for (int i = 0; i < BOARD_HEIGHT; i++)
      for (int j = 0; j < BOARD_WIDTH; j++)
        if (!board(i, j)) result += 1;
    return result;
  }

  int getColour() const {
	  return (moves % 2) ? -1 : 1;
  }
};

std::ostream& operator<<(std::ostream& os, const GameState& m) {
	return os << m.toString();
};


class Flippo {

    using VectorMatrix = std::vector<Matrix<double>>;
    using VectorGameState = std::vector<GameState>;

  public:

	static NeuralNetwork<double>* nn;

    static VectorGameState createGame() {

      std::default_random_engine generator(time(0));
      VectorGameState v;
      v.push_back(GameState());
      double c = 1.0;

      for (int k = 0; k < BOARD_SIZE - 4; k++) {
        std::vector<std::tuple<int, int>> moves = v.back().validMoves(c);
        std::uniform_int_distribution<int> distribution(0, moves.size()-1);
        auto [i, j] = moves[distribution(generator)];
        v.push_back(v.back().potentialBoard(i, j, c));
        c *= -1.0;
      }

      return v;
    }

	static std::tuple<int, int> predictMove(const GameState& s) {
		int c = s.getColour();
		Matrix<double> m = nn->min();
		auto moves = s.validMoves(c);
		int r = -1;
		for (unsigned int i = 0; i < moves.size(); i++) {
			auto [x, y] = moves[i];
			auto p = nn->evaluate(s.potentialBoard(x, y, c).input());
			if (c*p[0] > c*m[0]) {
				m = p;
				r = i;
			}
		}
		return moves[r];
	}

	static std::tuple<int, int> randomBenchmarkerSingle() {
		GameState white;
		GameState black;
		std::default_random_engine generator(time(0));
		for (int k= 0; k < BOARD_SIZE/2-2; k++) {
			// White AI
			auto [x1, y1] = Flippo::predictMove(white);
			white.placePiece(x1, y1, 1);
			// White Random
			std::vector<std::tuple<int, int>> moves1 = white.validMoves(-1);
			std::uniform_int_distribution<int> distribution1(0, moves1.size()-1);
			auto [i1, j1] = moves1[distribution1(generator)];
			white.placePiece(i1, j1, -1);
			// Black Random
			std::vector<std::tuple<int, int>> moves2 = black.validMoves(1);
			std::uniform_int_distribution<int> distribution2(0, moves2.size()-1);
			auto [i2, j2] = moves2[distribution2(generator)];
			black.placePiece(i2, j2, 1);
			// Black AI
			auto [x2, y2] = Flippo::predictMove(black);
			black.placePiece(x2, y2, -1);
		}
		int whiteScore = white.getScore();
		int blackScore = black.getScore() * -1;
		int wins = (whiteScore > 0 ? 1 : 0) + (blackScore > 0 ? 1 : 0);
		return {(whiteScore + blackScore), wins};
	}

	static std::tuple<double, double> randomBenchmarker(int n = 10) {
		int result = 0;
		int wins = 0;
		for (int i = 0; i < n; i++) {
			auto [score, win] = randomBenchmarkerSingle();
			result += score;
			wins += win;
		}
		return {(double)result / ((double)n * 2), (double)wins/((double)n * 2) * 100};
	}

	static Matrix<double> getTarget(GameState& s) {
		if (s.isFinal())
			return Matrix<double>(1, 1, {s.getScore()});

		int c = s.getColour();
		Matrix<double> m = nn->min();

		for (auto [i, j] : s.validMoves(c)) {
			auto p = nn->evaluate(s.potentialBoard(i, j, c).input());
			if (c*p[0] > c*m[0])
				m = p;
		}

		return m;
	}
};

NeuralNetwork<double>* Flippo::nn = nullptr;


#endif
