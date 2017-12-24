#include <iostream>

#include "InformationSet.h"

namespace cfr {

InformationSet::InformationSet(int actions) : actions(actions) {
  regretSum = new double[actions];
  strategySum = new double[actions];

  for (int i = 0; i < actions; i++) {
    regretSum[i] = 0.0;
    strategySum[i] = 0.0;
  }
}

InformationSet::InformationSet(int actions, double *regretSum, double *strategySum) :
  actions(actions), regretSum(regretSum), strategySum(strategySum) {}

InformationSet::~InformationSet() {
  delete regretSum;
  delete strategySum;
}

double *InformationSet::getStrategy() const {
  double normalizingSum = 0;
  double *strategy = new double[actions];
  for (int i = 0; i < actions; i++) {
    strategy[i] = regretSum[i] > 0 ? regretSum[i] : 0;
    normalizingSum += strategy[i];
  }
  for (int i = 0; i < actions; i++) {
    strategy[i] = normalizingSum > 0 ? strategy[i] / normalizingSum : 1.0 / actions;
  }
  return strategy;
}

double *InformationSet::getAverageStrategy() const {
  double normalizingSum = 0;
  double *avgStrategy = new double[actions];
  for (int i = 0; i < actions; i++) {
    normalizingSum += strategySum[i];
  }
  for (int i = 0; i < actions; i++) {
    avgStrategy[i] = normalizingSum > 0 ? strategySum[i] / normalizingSum : 1.0 / actions;
  }
  return avgStrategy;
}

void InformationSet::printAverageStrategy() const {
  double *avgStrategy = getAverageStrategy();

  for (int i = 0; i < actions; i++) {
    if (i != 0) {
      std::cout << " , ";
    }
    std::cout << avgStrategy[i];
  }
  std::cout << std::endl;

  delete avgStrategy;
}

void InformationSet::updateStrategySum(double realizationWeight, double *strategy) {
  if (strategy == NULL) {
    strategy = getStrategy();
  }
  for (int i = 0; i < actions; i++) {
    strategySum[i] += realizationWeight * strategy[i];
  }
}

}
