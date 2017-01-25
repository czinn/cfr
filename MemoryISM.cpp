#include <iostream>

#include "MemoryISM.h"

MemoryISM::MemoryISM() {}

MemoryISM::~MemoryISM() {
  map.clear();
}

InformationSet *MemoryISM::getInformationSet(std::string id, int actions) {
  if (map.count(id) > 0) {
    return map[id];
  } else {
    InformationSet *is = new InformationSet(actions);
    is->id = id;
    return is;
  }
}

void MemoryISM::saveInformationSet(InformationSet *is) {
  map[is->id] = is;
}

void MemoryISM::printNodes() const {
  for (auto it = map.begin(); it != map.end(); it++) {
    it->second->printAverageStrategy();
  }
}
