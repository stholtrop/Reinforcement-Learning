#ifndef NEURALNETWORK
#define NEURALNETWORK
#include "matrix.cpp"
#include <vector>


class NeuralNetwork {
  std::vector<Matrix<float>> layers;
  size_t input_size;
  size_t output_size;
public:
  NeuralNetwork(std::vector<size_t> sizes){
    input_size = sizes[0];
    output_size = *sizes.rbegin();


  }
};

#endif
