#include "flippo.cpp"
#include <iostream>

using namespace std;

int main() {
  cout << "Testing Flippo" << endl;
  GameState m;
  cout << m << endl << "Score" << endl << m.score() << endl;
  cout << "Placing a piece at [2, 4]" << endl;
  m.placePiece(4, 2, 1.0);
  cout << m << endl;
  // cout << "Testing potential board" << endl;
  // GameState next = m.potential_board(4, 1, 1);
  // cout << next << endl;
  cout << "Testing valid moves" << endl;
  Matrix<int> possible_moves = m.validMoves(-1);
  cout << possible_moves << endl;
}
