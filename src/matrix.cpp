#include <vector>
#include <cmath>

template <class T>
class Matrix {
private:
  std::vector<T> data;
  int rows;
  int columns;
  Matrix(int x, int y) {
    data(x * y);
    rows = x;
    columns = y;
  }
  T& operator[](int x){
    return &data[x];
  }
};
