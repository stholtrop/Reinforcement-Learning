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
}
