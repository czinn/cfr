#include <cassert>
#include <iostream>

#include "LoveLetterGame.h"

#define CARDS 16
#define PREREMOVE 3

namespace cfr {
namespace love {

LoveLetterGame::LoveLetterGame(long seed)
  : Game(seed), cards{GUARD, GUARD, GUARD, GUARD, GUARD, PRIEST, PRIEST, BARON, BARON, HANDMAID, HANDMAID, PRINCE, PRINCE, KING, COUNTESS, PRINCESS} {
  // Shuffle the deck
  for (int i = CARDS - 1; i > 0; i--) {
    int j = seed % (i + 1);
    seed /= i + 1;
    int tmp = cards[j];
    cards[j] = cards[i];
    cards[i] = tmp;
  }

  for (int i = 0; i < 2; i++) {
    hand[i] = cards[i + PREREMOVE];
    discardScore[i] = 0;
    eliminated[i] = false;
    known[i] = false;
  }
  top = 2 + PREREMOVE;
  for (int i = 0; i < 8; i++) {
    remaining[i] = 0;
  }
  for (int i = PREREMOVE; i < CARDS; i++) {
    remaining[cards[i] - 1]++;
  }
  handmaid = false;
  lastAction = 0;
  current = 0;
}

LoveLetterGame::LoveLetterGame(const LoveLetterGame &game)
    : Game(game)
    , top(game.top)
    , lastAction(game.lastAction)
    , handmaid(game.handmaid)
    , current(game.current) {
  for (int i = 0; i < CARDS; i++) {
    cards[i] = game.cards[i];
  }
  for (int i = 0; i < 2; i++) {
    discardScore[i] = game.discardScore[i];
    hand[i] = game.hand[i];
    eliminated[i] = game.eliminated[i];
    known[i] = game.known[i];
  }
  for (int i = 0; i < 8; i++) {
    remaining[i] = game.remaining[i];
  }
}

LoveLetterGame::~LoveLetterGame() {}

LoveLetterGame *LoveLetterGame::clone() const {
  LoveLetterGame *g = new LoveLetterGame(*this);
  return g;
}

int LoveLetterGame::currentPlayer() const {
  return current;
}

bool LoveLetterGame::isTerminal() const {
  return eliminated[0] || eliminated[1] || top >= CARDS - 1;
}

double LoveLetterGame::utility() const {
  if (eliminated[current]) return -1;
  if (eliminated[1 - current]) return 1;
  if (hand[current] > hand[1 - current]) return 1;
  if (hand[current] < hand[1 - current]) return -1;
  if (discardScore[current] > discardScore[1 - current]) return 1;
  if (discardScore[current] < discardScore[1 - current]) return -1;
  return 0; // wow, a tie actually happened?
}

std::string LoveLetterGame::getStateId() const {
  std::string s = "";
  for (int i = top; i < CARDS; i++) {
    s += (char)(cards[i] + '0');
  }
  s += (char)('A' + lastAction);
  for (int i = 0; i < 2; i++) {
    if (current == i) s += '.';
    s += (char)('0' + hand[i]);
    if (eliminated[i]) s += 'E';
    if (known[i]) s += 'K';
    s += (char)('A' + discardScore[i]);
  }
  if (handmaid) s += 'H';
  return s;
}

std::string LoveLetterGame::getInformationSetId() const {
  std::string s = currentHand() + ';';
  s[0] += '0';
  s[1] += '0';
  for (int i = 0; i < 8; i++) {
    s += (char)(remaining[i] + '0');
  }
  if (lastAction < 10) {
    s += lastAction + '0';
  } else {
    s += (char)(lastAction + 'A' - 10);
  }
  if (known[1 - current]) {
    s += ";";
    s += (char)(hand[1 - current] + '0');
  }
  if (known[current]) {
    s += ";";
    s += 'K';
  }
  return s;
}

std::string LoveLetterGame::currentHand() const {
  if (top >= CARDS) return "";
  char first = hand[current];
  char second = cards[top];
  return std::string(1, first) + std::string(1, second);
}

int LoveLetterGame::numActions() const {
  return listActions().size();
}

std::string LoveLetterGame::listActions() const {
  std::string h = currentHand();
  std::string actions = "";
  if (h.find(GUARD) != std::string::npos) {
    if (handmaid) {
      actions += std::string(1, GUARD);
    } else {
      for (char g = PRIEST; g <= PRINCESS; g++) {
        actions += std::string(1, GUESS + g);
      }
    }
  }
  if (h.find(PRIEST) != std::string::npos) {
    actions += std::string(1, PRIEST);
  }
  if (h.find(BARON) != std::string::npos) {
    actions += std::string(1, BARON);
  }
  if (h.find(HANDMAID) != std::string::npos) {
    actions += std::string(1, HANDMAID);
  }
  if (h.find(PRINCE) != std::string::npos) {
    if (h.find(COUNTESS) == std::string::npos) {
      if (!handmaid) {
        actions += std::string(1, PRINCE);
      }
      actions += std::string(1, PRINCE_SELF);
    }
  }
  if (h.find(KING) != std::string::npos) {
    if (h.find(COUNTESS) == std::string::npos) {
      actions += std::string(1, KING);
    }
  }
  if (h.find(COUNTESS) != std::string::npos) {
    actions += std::string(1, COUNTESS);
  }
  return actions;
}

void LoveLetterGame::makeAction(int action) {
  std::string actions = listActions();
  char a = actions[action];

  // update the player's remaining card and shift top down
  char card = a;
  if (card >= GUESS + PRIEST && card <= GUESS + PRINCESS) {
    card = GUARD;
  } else if (card == PRINCE_SELF) {
    card = PRINCE;
  }
  remaining[card - 1]--;
  if (hand[current] == card) {
    hand[current] = cards[top];
    known[current] = false;
  }
  top++;

  // execute action
  if (a >= GUESS + PRIEST && a <= GUESS + PRINCESS) {
    if (a - GUESS == hand[1 - current]) {
      eliminated[1 - current] = true;
    }
  } else if (a == PRIEST) {
    if (!handmaid) {
      known[1 - current] = true;
    }
  } else if (a == BARON) {
    if (!handmaid) {
      if (hand[current] > hand[1 - current]) eliminated[1 - current] = true;
      else if (hand[current] < hand[1 - current]) eliminated[current] = true;
      else {
        known[0] = true;
        known[1] = true;
      }
    }
  } else if (a == PRINCE) {
    if (hand[1 - current] == PRINCESS) eliminated[1 - current] = true;
    remaining[hand[1 - current] - 1]--;
    hand[1 - current] = cards[top];
    top++;
  } else if (a == PRINCE_SELF) {
    if (hand[current] == PRINCESS) eliminated[current] = true;
    remaining[hand[current] - 1]--;
    hand[current] = cards[top];
    top++;
  } else if (a == KING) {
    if (!handmaid) {
      char tmp = hand[current];
      hand[current] = hand[1 - current];
      hand[1 - current] = tmp;
      known[0] = true;
      known[1] = true;
    }
  }
  lastAction = a;
  current = 1 - current;
  handmaid = a == HANDMAID;
}

}
}
