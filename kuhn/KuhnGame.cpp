#include "KuhnGame.h"

namespace cfr {
namespace kuhn {

KuhnGame::KuhnGame(long seed) : Game(seed) {
  for (int i = 0; i < CARDS; i++) {
    cards[i] = i + 1;
  }
  for (int i = CARDS - 1; i > 0; i--) {
    int j = seed % (i + 1);
    seed /= i + 1;
    int tmp = cards[j];
    cards[j] = cards[i];
    cards[i] = tmp;
  }

  history = "";
  current = 0;
}

KuhnGame::KuhnGame(const KuhnGame &game) : Game(game), history(game.history), current(game.current) {
  for (int i = 0; i < CARDS; i++) {
    cards[i] = game.cards[i];
  }
}

KuhnGame::~KuhnGame() {}

KuhnGame *KuhnGame::clone() const {
  KuhnGame *g = new KuhnGame(*this);
  return g;
}

int KuhnGame::currentPlayer() const {
  return current;
}

bool KuhnGame::isTerminal() const {
  if (history.length() <= 1) {
    return false;
  }
  bool terminalPass = history[history.length() - 1] == 'p';
  bool doubleBet = history.substr(history.length() - 2, 2) == "bb";
  return terminalPass || doubleBet;
}

double KuhnGame::utility() const {
  bool terminalPass = history[history.length() - 1] == 'p';
  bool doubleBet = history.substr(history.length() - 2, 2) == "bb";
  bool isPlayerCardHigher = cards[current] > cards[1 - current];

  if (terminalPass) {
    if (history == "pp") {
      return isPlayerCardHigher ? 1 : -1;
    } else {
      return 1;
    }
  } else if (doubleBet) {
    return isPlayerCardHigher ? 2 : -2;
  }
  // shouldn't get here
  return 0;
}

std::string KuhnGame::getStateId() const {
  return std::to_string(cards[0]) + std::to_string(cards[1]) + history;
}

std::string KuhnGame::getInformationSetId() const {
  return std::to_string(cards[current]) + history;
}

int KuhnGame::numActions() const {
  return 2;
}

void KuhnGame::makeAction(int action) {
  history += action == 0 ? "p" : "b";
  current = 1 - current;
}

}
}
