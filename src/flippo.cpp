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

  GameState() {
    int middle_x = BOARD_WIDTH/2-1;
    int middle_y = BOARD_HEIGHT/2-1;
    board = Matrix<double>(BOARD_HEIGHT, BOARD_WIDTH);
    board(middle_y, middle_x) = 1;
    board(middle_y+1, middle_x + 1) = 1;
    board(middle_y, middle_x + 1) = -1;
    board(middle_y+1, middle_x) = -1;
  }

  double score() {
    double result = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
      result += board[i];
    }
    return result;
  }

  std::string toString() const {
    return board.toString();
  }
};

std::ostream& operator<<(std::ostream& os, const GameState& m) {
	return os << m.toString();
};

class Flippo {
public:
  GameState state;
  int moves = 4;

  void place_piece(int i, int j, double c) {
    state.board(i, j) = c;
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

  void update(int i, int j, int di, int dj, double c) {
    // Update along a certain direction given by di, dj
    auto [fi, fj] = find_flip(i, j, di, dj, c);
    if (fi == i && fj == j) {
      return;
    }
    fi -= di;
    fj -= dj;
    while (fi != i || fj != j) {
      state.board(fi, fj) *= -1.0;
      fi -= di;
      fj -= dj;
    }
  }

  inline bool inbound(int i, int j) {
    return (0 <= i) && (i <  BOARD_HEIGHT) && (0 <= j) && (j < BOARD_WIDTH);
  }

  std::tuple<int, int> find_flip(int i, int j, int di, int dj, double c) {
    // Find flip of other color along direction di, dj
    int fi = i;
    int fj = j;
    while (inbound(i, j)) {
      if (state.board(i, j) == 0) {
        break;
      }
      if (state.board(i, j) == c) {
        fi = i;
        fj = j;
      }
      i += di;
      j += dj;
    }
    return {fi, fj};
  }

  double inline score() {
    return state.score();
  }

  bool is_final() {
    return moves == BOARD_SIZE;
  }
};
#endif
