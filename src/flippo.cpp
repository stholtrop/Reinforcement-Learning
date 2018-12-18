#ifndef FLIPPO
#define FLIPPO
#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include "matrix.cpp"
#include <iostream>

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

	double score() const {
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

	int valid(const int i, const int j, const double c) const {

		if (board(i, j) != 0) return 0;

		if (!hasNeighbours(i, j)) return 0;

		for (int di = -1; di < 2; di++) {

			for (int dj = -1; dj < 2; dj++) {

				if (di == 0 && dj == 0) continue;
				auto [fi, fj] = getFlip(i, j, di, dj, c);
				if ((fi != i || fj != j) && (fi != i + di || fj != j + dj)) return 2;

			}
		}

		return 1;
	}

	Matrix<int> validMoves(const double c) const {
		Matrix<int> move_matrix(BOARD_HEIGHT, BOARD_WIDTH);
		for (int i = 0; i < BOARD_WIDTH; i++) {
			for (int j = 0; j < BOARD_HEIGHT; j++) {
				move_matrix(i, j) = valid(i, j, c);
			}
		}
		return move_matrix;
	}

	GameState potentialBoard(const int i, const int j, const double c) const {

		GameState copy(*this);
		copy.placePiece(i, j, c);
		return copy;

	}

	inline bool isFinal() const {
		return moves == BOARD_SIZE;
	}


	std::string toString() const {
		return board.toString();
	}
};

std::ostream& operator<<(std::ostream& os, const GameState& m) {
	return os << m.toString();
};

class Flippo {

};
#endif
