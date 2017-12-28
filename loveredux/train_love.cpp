#include <chrono>
#include <iostream>
#include <random>

#include "LoveLetterGame.h"
#include "../LevelDBISM.h"
#include "../cfr.h"

namespace cfr {
namespace love {

void train(int iterations) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 mt(seed);
  double util = 0;
  LevelDBISM *m = new LevelDBISM("love.db");
  for (int i = 0; i < iterations; i++) {
    if (i % 100 == 0) {
      std::cout << i << std::endl;
    }
    LoveLetterGame *g = new LoveLetterGame(((long)(mt()) << sizeof(int)) | mt());
    util += dpcfr(m, g);
    delete g;
  }

  std::cout << "Average game value: " << util / iterations << std::endl;

  delete m;
}

}
}

int main() {
  cfr::love::train(1000);
}
