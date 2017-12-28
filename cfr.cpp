#include <deque>
#include <unordered_map>
#include <iostream>

#include "cfr.h"
#include "InformationSet.h"

namespace cfr {

// Game must be two-player, zero-sum.
double cfr(InformationSetManager *m, Game *g, double p0, double p1) {
  if (g->isTerminal()) {
    return g->utility();
  }

  int player = g->currentPlayer();

  int numActions = g->numActions();
  double *strategy;
  InformationSet *is = NULL;
  if (numActions > 1) {
    is = m->getInformationSet(g->getInformationSetId(), numActions);
    strategy = is->getStrategy();
    is->updateStrategySum(player == 0 ? p0 : p1, strategy);
  } else {
    strategy = new double[numActions];
    strategy[0] = 1.0;
  }

  double util[numActions];
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

  if (numActions > 1) {
    for (int i = 0; i < numActions; i++) {
      double regret = util[i] - nodeUtil;
      is->regretSum[i] += (player == 0 ? p1 : p0) * regret;
    }

    m->saveInformationSet(is);
  }

  delete strategy;

  return nodeUtil;
}

typedef struct node {
  double p[2];
  double utility;
  int visits;
} node;

void toposort(Game *g, std::unordered_map<std::string, node> &visited,
              std::deque<Game *> &games) {
  std::string id = g->getStateId();
  if (visited.count(id) != 0) {
    return;
  }
  visited[id] = node{{0.0, 0.0}, 0.0, 0};
  int numActions = g->numActions();
  for (int i = 0; i < numActions; i++) {
    Game *ng = g->clone();
    ng->makeAction(i);
    toposort(ng, visited, games);
  }
  games.push_front(g);
}

// Game must be two-player, zero-sum, and game states must be acyclic.
double dpcfr(InformationSetManager *m, Game *g) {
  std::unordered_map<std::string, node> nodes;
  std::unordered_map<std::string, InformationSet *> isets;
  std::deque<Game *> games;
  // Sort games
  toposort(g, nodes, games);
  // Forward pass
  nodes[g->getStateId()] = node{{1.0, 1.0}, 0.0, 1};
  for (auto it = games.begin(); it != games.end(); it++) {
    Game *ig = *it;
    node n = nodes[ig->getStateId()];
    if (ig->isTerminal()) {
      continue;
    }
    int numActions = ig->numActions();
    double *strategy;
    if (numActions > 1) {
      std::string isid = ig->getInformationSetId();
      InformationSet *is;
      if (isets.count(isid) == 0) {
        is = m->getInformationSet(isid, numActions);
        isets[isid] = is;
      } else {
        is = isets[isid];
      }
      strategy = is->getStrategy();
      //is->updateStrategySum(n.p[ig->currentPlayer()] * n.visits, strategy);
      is->updateStrategySum(n.p[ig->currentPlayer()], strategy);
    } else {
      strategy = new double[numActions];
      strategy[0] = 1.0;
    }
    // Iterate through children and update their nodes.
    for (int i = 0; i < numActions; i++) {
      Game *ng = ig->clone();
      ng->makeAction(i);
      node nn = nodes[ng->getStateId()];
      nn.visits += n.visits;
      for (int j = 0; j < 2; j++) {
        nn.p[j] += n.p[j] * (ig->currentPlayer() == j ? strategy[i] : 1.0);
      }
      nodes[ng->getStateId()] = nn;
      delete ng;
    }
    delete strategy;
  }
  // Backward pass
  for (auto it = games.rbegin(); it != games.rend(); it++) {
    Game *ig = *it;
    node n = nodes[ig->getStateId()];
    if (ig->isTerminal()) {
      n.utility = ig->utility();
      nodes[ig->getStateId()] = n;
      continue;
    }
    int numActions = ig->numActions();
    double *strategy;
    InformationSet *is;
    if (numActions > 1) {
      is = isets[ig->getInformationSetId()];
      strategy = is->getStrategy();
      //is->updateStrategySum(n.p[ig->currentPlayer()] * n.visits, strategy);
      is->updateStrategySum(n.p[ig->currentPlayer()], strategy);
    } else {
      strategy = new double[numActions];
      strategy[0] = 1.0;
    }
    double util[numActions];
    n.utility = 0.0;
    // Iterate through children and backpropagate their utilities
    for (int i = 0; i < numActions; i++) {
      Game *ng = ig->clone();
      ng->makeAction(i);
      node nn = nodes[ng->getStateId()];
      util[i] = nn.utility;
      n.utility += nn.utility * strategy[i];
      delete ng;
    }
    nodes[ig->getStateId()] = n;
    if (numActions > 1) {
      for (int i = 0; i < numActions; i++) {
        is->regretSum[i] += n.p[1 - ig->currentPlayer()] * (util[i] - n.utility);
      }
    }
    delete strategy;
  }
  // Save all information sets
  for (auto it = isets.begin(); it != isets.end(); it++) {
    m->saveInformationSet(it->second);
  }
  // Return utility of root state
  return nodes[g->getStateId()].utility;
}

}
