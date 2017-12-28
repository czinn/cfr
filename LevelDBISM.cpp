#include <cassert>
#include <iostream>

#include "LevelDBISM.h"

namespace cfr {

LevelDBISM::LevelDBISM(std::string db_path) {
  options.create_if_missing = true;
  leveldb::Status s = leveldb::DB::Open(options, db_path, &db);
  assert(s.ok());
}

LevelDBISM::~LevelDBISM() {
  delete db;
}

InformationSet *decodeSet(std::string value, int actions) {
  const char *v = value.c_str();
  if (actions == 0) {
    actions = value.size() / 16;
  }
  double *regretSum = new double[actions];
  double *strategySum = new double[actions];
  for (int i = 0; i < actions; i++) {
    regretSum[i] = ((double*)v)[i];
    strategySum[i] = ((double*)v)[i + actions];
  }
  return new InformationSet(actions, regretSum, strategySum);
}

InformationSet *LevelDBISM::getInformationSet(std::string id, int actions) {
  std::string value;
  leveldb::Status s = db->Get(leveldb::ReadOptions(), id, &value);
  InformationSet *is;
  if (s.ok()) {
    is = decodeSet(value, actions);
  } else {
    is = new InformationSet(actions);
  }
  is->id = id;
  return is;
}

void LevelDBISM::saveInformationSet(InformationSet *is) {
  int actions = is->actions;
  double *data = new double[actions * 2];
  for (int i = 0; i < actions; i++) {
    data[i] = is->regretSum[i];
    data[i + actions] = is->strategySum[i];
  }
  std::string value((char*)data, actions * 2 * sizeof(double));
  leveldb::Status s = db->Put(leveldb::WriteOptions(), is->id, value);
  assert(s.ok());
  delete data;
  delete is; // kind of bad?
}

void LevelDBISM::listKeys() {
  int count = 0;
  leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    std::cout << it->key().ToString() << std::endl;
    count++;
  }
  std::cout << "Number of states: " << count << std::endl;
  delete it;
}

void LevelDBISM::listSets() {
  int count = 0;
  leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    std::cout << it->key().ToString() << '\t';
    InformationSet *is = decodeSet(it->value().ToString(), 0);
    is->printAverageStrategy();
    delete is;
    count++;
  }
  std::cout << "Number of states: " << count << std::endl;
  delete it;
}

}
