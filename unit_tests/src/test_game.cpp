#include "flippo.cpp"
#include <iostream>

using namespace std;

int main() {
  cout << "Testing Flippo" << endl;
  auto m = Flippo();
  cout << m.state.board << endl << "Score" << endl << m.state.score() << endl;
  cout << "Placing a piece at [2, 4]" << endl;
  m.place_piece(4, 2, 1.0);
  cout << m.state.board << endl;
  // cout << "Testing potential board" << endl;
  // GameState next = m.potential_board(4, 1, 1);
  // cout << next << endl;
  cout << "Testing valid moves" << endl;
  Matrix<int> possible_moves = m.valid_moves(-1);
  cout << possible_moves << endl;
}
