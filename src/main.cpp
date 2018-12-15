#include <iostream>
#include <cmath>
#include <fstream>
#include "matrix.cpp"
#include "neural-network.cpp"
#include "activators.cpp"


using namespace std;

template <typename T>
std::vector<Matrix<T>> to_vector_matrix(Matrix<T> in) {
  std::vector<Matrix<T>> new_matrix;
  for (unsigned int i = 0; i < in.rows; i++) {
    new_matrix.push_back(in.getRow(i).transpose());
  }
  return new_matrix;
}

int main(){
  auto input = to_vector_matrix(Matrix<double>::readFromFile("./data/mnist_test_input.ssv"));
  cout << "Stuff1" << endl;
  auto output = to_vector_matrix(Matrix<double>::readFromFile("./data/mnist_test_output.ssv"));
  cout << "Stuff2" << endl;
  NeuralNetwork<double> nn = NeuralNetwork<double>({input[0].rows, 30, output[0].rows}, new Sigmoid<double>());
  cout << "Stuff3" << endl;
  cout << output[0] << endl << "Prediction: " << endl << nn.evaluate(input[0]) << endl;
  nn.train(input, output, 10);
  cout << output[0] << endl << "Prediction: " << endl << nn.evaluate(input[0]) << endl;
}
