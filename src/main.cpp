#include <iostream>
#include "qlearner.cpp"
#include "neural-network.cpp"
#include "flippo.cpp"
#include "activators.cpp"
#include <csignal>
#include <ctime>
#include <string>

using namespace std;

QLearner *ql;

void gracefulExit(int signum) {
  cout << endl << "Shutting down, saving session..." << endl;
  string filename;
  filename = "saves/EmergencySave" + std::to_string(time(0)) + ".ssvn";
  ql->save(filename);
  cout << "Session saved" << endl;
  exit(signum);
}


int main(int argc, char **argv) {
  cout << "Start" << endl;
  // Register signal handler for graceful exit and emergency save
  signal(SIGINT, gracefulExit);
  NeuralNetwork<double> *nn;
  Function<double> *s = new Sigmoid<double>(), *l = new Linear<double>();
  // Parse command line options
  char *filename = nullptr;
  if (argc ==  2) {
    cout << "Result of training will be saved in " << argv[1] << endl;
    filename = argv[1];
    nn = new NeuralNetwork<double>({64, 32, 1}, s, l);
  }else if (argc == 3) { // This part caused a segfault
    cout << "Recover from " << argv[1] << " and save result in " << argv[2] << endl;
    filename = argv[2];
    nn = new NeuralNetwork<double>(string(argv[1])); // Constructor is working (see test), I suspect it's here
  } else {
    nn = new NeuralNetwork<double>({64, 32, 1}, s, l);
  }
  ql = new QLearner(nn, 0.9);
  cout << "Initialized qlearner" << endl;
  auto [score2, wins2] = Flippo::randomBenchmarker(5000);
  cout << "Finished benchmark" << endl;
  cout << "Avg score: " << score2 << " Percentage won: " << wins2 << endl;
  ql->initialize(5000, 50, 0.1, true);
  auto [score, wins] = Flippo::randomBenchmarker(5000);
  cout << "Avg score: " << score << " Percentage won: " << wins << endl;
  if (filename != nullptr) {
    cout << "Saving results..." << endl;
    ql->save(string(filename));
    cout << "Done saving" << endl;
  }else {
    cout << "Discarded training" << endl;
  }
  delete s; delete l; delete nn; delete ql;
}
