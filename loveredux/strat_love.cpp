#include <cassert>
#include <chrono>
#include <iostream>
#include <random>

#include "LoveLetterGame.h"
#include "../LevelDBISM.h"
#include "../cfr.h"

int main() {
  cfr::LevelDBISM ism("love.db");
  ism.listKeys();
  while (true) {
    std::string id;
    std::cin >> id;
    cfr::InformationSet *is = ism.getInformationSet(id, 0);
    is->printAverageStrategy();

    delete is;
  }
}
