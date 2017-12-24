#ifndef INFORMATION_SET_MANAGER_H
#define INFORMATION_SET_MANAGER_H

#include "InformationSet.h"

namespace cfr {

class InformationSetManager {
public:
  InformationSetManager();
  virtual ~InformationSetManager();

  virtual InformationSet *getInformationSet(std::string id, int actions) = 0;
  virtual void saveInformationSet(InformationSet *) = 0;
};

}

#endif
