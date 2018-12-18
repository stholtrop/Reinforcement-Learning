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

  double score() const {
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

  void place_piece(const int i, const int j, const double c) {
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

  void update(const int i, const int j, const int di, const int dj, const double c) {
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

  std::vector<std::tuple<int, int>> get_neighbours(const int i, const int j) const {
    std::vector<std::tuple<int, int>> neighbours;
    for (int di = -1; di < 2; di++) {
      for (int dj = -1; dj < 2; dj++) {
        int ni = i + di;
        int nj = j + dj;
        if (inbound(ni, nj)) {
          if (state.board(ni, nj) != 0){
            neighbours.push_back({ni, nj});
          }
        }
      }
    }
    return neighbours;
  }

  inline bool inbound(const int i, const int j) const {
    return (0 <= i) && (i <  BOARD_HEIGHT) && (0 <= j) && (j < BOARD_WIDTH);
  }

  std::tuple<int, int> find_flip(int i, int j, const int di, const int dj, const double c) const {
    // Find flip of color along direction di, dj
    i += di;
    j += dj;
    int fi = i + di;
    int fj = j + dj;
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

  GameState potential_board(const int i, const int j, const double c) const {
    Flippo copy(*this);
    copy.place_piece(i, j, c);
    return copy.state;
  }

  int valid(const int i, const int j, const double c) const {
    if (state.board(i, j) != 0) {
      return 0;
    }
    std::vector<std::tuple<int, int>> neighbours = get_neighbours(i, j);
    if (neighbours.size() == 0) {
      return 0;
    }
    for (int di = -1; di < 2; di++) {
      for (int dj = -1; dj < 2; dj++) {
        if (di == 0 && dj == 0) {
          continue;
        }
        auto [fi, fj] = find_flip(i, j, di, dj, c);
        std::cout << fi << " " << fj << " " << i << " " << j << std::endl;
        if ((fi != i || fj != j) && (fi != i + di || fj != j + dj)) {
          std::cout << "Found a move" << std::endl;
          return 2;
        }
      }
    }
    return 1;
  }

  Matrix<int> valid_moves(const double c) const {
    Matrix<int> move_matrix(BOARD_HEIGHT, BOARD_WIDTH);
    for (int i = 0; i < BOARD_WIDTH; i++) {
      for (int j = 0; j < BOARD_HEIGHT; j++) {
        move_matrix(i, j) = valid(i, j, c);
      }
    }
    return move_matrix;
  }

  inline double score() const {
    return state.score();
  }

  inline bool is_final() const {
    return moves == BOARD_SIZE;
  }
};
#endif
