#include <chrono>
#include <iostream>
#include <random>

#include "KuhnGame.h"
#include "LevelDBISM.h"
#include "cfr.h"

void train(int iterations) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 mt(seed);
  double util = 0;
  LevelDBISM *m = new LevelDBISM("kuhn.db");
  for (int i = 0; i < iterations; i++) {
    KuhnGame *g = new KuhnGame(mt());
    util += cfr(m, g, 1, 1);
    delete g;
  }

  std::cout << "Average game value: " << util / iterations << std::endl;
  for (int i = 1; i <= 9; i++) {
    InformationSet *is = m->getInformationSet(std::string(1, '0' + i), 2);
    is->printAverageStrategy();
    delete is;
  }

  delete m;
}

int main() {
  train(100000);
}
