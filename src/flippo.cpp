#ifndef FLIPPO
#define FLIPPO
#include <vector>
#include <string>
#include <sstream>

const int BOARD_WIDTH = 8;
const int BOARD_HEIGHT = 8;
const int BOARD_SIZE = BOARD_WIDTH * BOARD_HEIGHT;


class GameState {
public:
  double board[BOARD_SIZE] = {0};

  double score() {
    double result = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
      result += board[i];
    }
    return result;
  }

  std::string toString() const {
    std::ostringstream oss;
    oss << "GameState" << std::endl;
    for (unsigned int i = 0; i < BOARD_HEIGHT; i++) {
      for (unsigned int j = 0; j < BOARD_WIDTH; j++)
        oss << board[i * BOARD_HEIGHT + j] << ' ';
      if (i != BOARD_WIDTH - 1)
        oss << '\n';
    }
    return oss.str();
  }
};

std::ostream& operator<<(std::ostream& os, const GameState& m) {
	return os << m.toString();
}

class Flippo {
  
};
#endif
