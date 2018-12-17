#include "flippo.cpp"
#include <iostream>

using namespace std;

int main() {
  cout << "Testing Flippo" << endl;
  auto m = GameState();
  m.board[0] = -1;
  cout << m << endl << "Score" << endl << m.score();
}
