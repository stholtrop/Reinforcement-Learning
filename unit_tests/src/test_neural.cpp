#include "neural-network.cpp"
#include "activators.cpp"
#include "matrix.cpp"
#include "regularizers.cpp"
#include <fstream>

using namespace std;

int main() {
    NeuralNetwork<double> nn = NeuralNetwork<double>({1, 32, 1}, new Sigmoid<double>(), new Linear<double>(), nullptr);
    cout << nn.evaluate(Matrix<double>(1, 1, {1})) << endl;
    nn.saveNetwork("test.ssvn");
    NeuralNetwork<double> nn2;
    nn2.readNetwork("test.ssvn");
    cout << nn2.evaluate(Matrix<double>(1, 1, {1})) << endl;
    NeuralNetwork<double> nn3("test.ssvn");
    cout << nn3.evaluate(Matrix<double>(1, 1, {1})) << endl;
    // testdata
    std::vector<Matrix<double>> data = std::vector({Matrix<double>(1, 1, {0}), Matrix<double>(1, 1, {1})});
    std::vector<Matrix<double>> target = std::vector({Matrix<double>(1, 1, {0}), Matrix<double>(1, 1, {1})});
    NeuralNetwork<double> nn4 = NeuralNetwork<double>({1, 5, 1}, new Linear<double>(), new Linear<double>(), new BasicRegularizer<double>(0.0001));
    nn4.train(data, target, 500, 2, 0.01, true);
    cout << nn4.evaluate(data[0]) << endl;
    nn4.printNetwork();
}
