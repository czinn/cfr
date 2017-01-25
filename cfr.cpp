#include "cfr.h"
#include "InformationSet.h"

double cfr(InformationSetManager *m, Game *g, double p0, double p1) {
  if (g->isTerminal()) {
    return g->utility();
  }

  int player = g->currentPlayer();

  int numActions = g->numActions();
  InformationSet *is = m->getInformationSet(g->getInformationSetId(), numActions);

  double *strategy = is->getStrategy();
  is->updateStrategySum(player == 0 ? p0 : p1, strategy);

  double *util = new double[numActions];
  double nodeUtil = 0;

  for (int i = 0; i < numActions; i++) {
    Game *ng = g->clone();
    ng->makeAction(i);
    util[i] = player == 0
      ? - cfr(m, ng, p0 * strategy[i], p1)
      : - cfr(m, ng, p0, p1 * strategy[i]);
    nodeUtil += strategy[i] * util[i];
    delete ng;
  }

  for (int i = 0; i < numActions; i++) {
    double regret = util[i] - nodeUtil;
    is->regretSum[i] += (player == 0 ? p1 : p0) * regret;
  }

  m->saveInformationSet(is);

  return nodeUtil;
}
