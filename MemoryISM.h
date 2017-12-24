#ifndef MEMORY_ISM_H
#define MEMORY_ISM_H

#include <map>
#include "InformationSetManager.h"

namespace cfr {

class MemoryISM : public InformationSetManager {
public:
  MemoryISM();
  virtual ~MemoryISM();

  virtual InformationSet *getInformationSet(std::string id, int actions) override;
  virtual void saveInformationSet(InformationSet *) override;
  
  void printNodes() const;

private:
  std::map<std::string, InformationSet *> map;
};

}

#endif
