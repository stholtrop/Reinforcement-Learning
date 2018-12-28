#include "neural-network.cpp"
#include "activators.cpp"
#include "matrix.cpp"
#include <fstream>

using namespace std;

int main() {
    NeuralNetwork<double> nn = NeuralNetwork<double>({1, 32, 1}, new Sigmoid<double>(), new Linear<double>());
    cout << nn.evaluate(Matrix<double>(1, 1, {1})) << endl;
    nn.saveNetwork("test.ssvn");
    NeuralNetwork<double> nn2;
    nn2.readNetwork("test.ssvn");
    cout << nn2.evaluate(Matrix<double>(1, 1, {1})) << endl;
}