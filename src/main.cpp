#include <iostream>
#include <cmath>
#include <fstream>
#include "matrix.cpp"
#include "neural-network.cpp"
#include "activators.cpp"


using namespace std;

int main(){
	Sigmoid<double>* sigma = new Sigmoid<double>();
	NeuralNetwork<double> nn1(vector<size_t>{2, 3, 1}, sigma);
	ofstream fout("./NeuralNet1");
	nn1.write(fout);
	fout.close();
	ifstream fin("./NeuralNet1");
	NeuralNetwork<double> nn2 = NeuralNetwork<double>::load(fin, sigma);
	fin.close();
	cout << nn2.inputSize << ' ' << nn2.outputSize << endl;
	for (unsigned int i = 0; i < nn2.weights.size(); i++) {
		cout << nn2.weights[i] << endl;
		cout << nn2.biases[i] << endl;	
	}
}
