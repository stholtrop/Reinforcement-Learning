#ifndef NEURALNETWORK
#define NEURALNETWORK
#include "matrix.cpp"
#include <vector>


class NeuralNetwork {
  std::vector<Matrix<float>> layers;
  size_t input_size;
  size_t output_size;
public:
  NeuralNetwork(std::vector<size_t> sizes): layers(sizes.size()) {
    input_size = sizes[0];
    output_size = *sizes.rbegin();
    size_t prev = input_size;
    for (size_t i = 0; i < sizes.size()-1; i++){
      layers[i] = Matrix<float>::initializeRandom(prev, sizes[i+1]);
      prev = sizes[i+1];
    }
  }
    Matrix<float> predict(Matrix<float> &input){
      Matrix<float> intermediate = input;
      for (auto i : layers){
        std::cout << intermediate.columns << " " << i.rows << std::endl;
        intermediate *= i;
      }
      return intermediate;
  }
};

#endif
