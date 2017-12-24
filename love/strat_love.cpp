#include <cassert>
#include <chrono>
#include <iostream>
#include <random>

#include "LoveLetterGame.h"
#include "../LevelDBISM.h"
#include "../cfr.h"

namespace cfr {
namespace love {

int main() {
  LevelDBISM ism("love.db");
  while (true) {
    std::string id;
    std::cin >> id;
    for (int i = 0; i < id.size(); i++) {
      if ('0' <= id[i] && id[i] <= '9') {
        id[i] = (char)(id[i] - '0');
      } else {
        id[i] = (char)(id[i] - 'A' + 10);
      }
    }
    InformationSet *is = ism.getInformationSet(id, 0);
    is->printAverageStrategy();

    delete is;
  }
}

}
}
