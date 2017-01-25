#include <iostream>
#include <random>

#include "KuhnGame.h"
#include "MemoryISM.h"
#include "cfr.h"

void train(int iterations) {
  std::mt19937 mt;
  double util = 0;
  MemoryISM *m = new MemoryISM;
  for (int i = 0; i < iterations; i++) {
    KuhnGame *g = new KuhnGame(mt());
    util += cfr(m, g, 1, 1);
    delete g;
  }

  std::cout << "Average game value: " << util / iterations << std::endl;
  m->printNodes();

  delete m;
}

int main() {
  train(10000000);
}
