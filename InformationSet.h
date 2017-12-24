#ifndef INFORMATION_SET_H
#define INFORMATION_SET_H

#include <string>

namespace cfr {

class InformationSet {
public:
  InformationSet(int actions);
  InformationSet(int actions, double *regretSum, double *strategySum);
  virtual ~InformationSet();

  double *getStrategy() const;
  double *getAverageStrategy() const;
  void printAverageStrategy() const;

  void updateStrategySum(double realizationWeight, double *strategy);

  std::string id;
  int actions;
  double *regretSum;
  double *strategySum;
};

}

#endif
