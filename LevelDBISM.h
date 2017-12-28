#ifndef LEVELDB_ISM_H
#define LEVELDB_ISM_H

#include <string>

#include "leveldb/db.h"
#include "InformationSetManager.h"

namespace cfr {

class LevelDBISM : public InformationSetManager {
public:
  LevelDBISM(std::string db_path);
  virtual ~LevelDBISM();

  virtual InformationSet *getInformationSet(std::string id, int actions) override;
  virtual void saveInformationSet(InformationSet *) override;
  void listKeys();
  void listSets();

private:
  leveldb::DB* db;
  leveldb::Options options;
};

}

#endif
