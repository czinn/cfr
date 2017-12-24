#include "LoveLetterGame.h"

#define CARDS 13

namespace cfr {
namespace love {

LoveLetterGame::LoveLetterGame(long seed)
  : Game(seed), cards{GUARD, GUARD, GUARD, /*GUARD, GUARD, PRIEST,*/ PRIEST, BARON, BARON, HANDMAID, HANDMAID, PRINCE, PRINCE, KING, COUNTESS, PRINCESS} {
  // Shuffle the deck
  for (int i = CARDS - 1; i > 0; i--) {
    int j = seed % (i + 1);
    seed /= i + 1;
    int tmp = cards[j];
    cards[j] = cards[i];
    cards[i] = tmp;
  }

  top = 2 + 0;
  history = "";//std::string(cards + 2, 3); // three revealed cards
  std::sort(history.begin(), history.end());
  for (int i = 0; i < 2; i++) {
    hand[i] = cards[i];
    discardScore[i] = 0;
    eliminated[i] = false;
    handmaid[i] = false;
  }

  current = 0;
}

LoveLetterGame::LoveLetterGame(const LoveLetterGame &game)
  : Game(game), top(game.top), history(game.history), current(game.current) {
  for (int i = 0; i < CARDS; i++) {
    cards[i] = game.cards[i];
  }
  for (int i = 0; i < 2; i++) {
    discardScore[i] = game.discardScore[i];
    hand[i] = game.hand[i];
    eliminated[i] = game.eliminated[i];
    handmaid[i] = game.handmaid[i];
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

std::string LoveLetterGame::getInformationSetId() const {
  return history + currentHand();
}

std::string LoveLetterGame::currentHand() const {
  if (top >= CARDS) return "";
  char first = hand[current];
  char second = cards[top];
  if (first <= second) {
    return std::string(1, first) + std::string(1, second);
  } else {
    return std::string(1, second) + std::string(1, first);
  }
}

int LoveLetterGame::numActions() const {
  return listActions().size();
}

std::string LoveLetterGame::listActions() const {
  std::string h = currentHand();
  std::string actions = "";
  if (h.find(GUARD) != std::string::npos) {
    if (handmaid[1 - current]) {
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
      if (!handmaid[1 - current]) {
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

  history += std::string(1, a);

  // update the player's remaining card and shift top down
  char card = a;
  if (card >= GUESS + PRIEST && card <= GUESS + PRINCESS) {
    card = GUARD;
  } else if (card == PRINCE_SELF) {
    card = PRINCE;
  }
  if (hand[current] == card) {
    hand[current] = cards[top];
  }
  top++;

  // execute action
  if (a >= GUESS + PRIEST && a <= GUESS + PRINCESS) {
    if (a - GUESS == hand[1 - current]) {
      eliminated[1 - current] = true;
    }
  } else if (a == PRIEST) {
    if (!handmaid[1 - current]) {
      history += std::string(1, hand[1 - current]);
    }
  } else if (a == BARON) {
    if (!handmaid[1 - current]) {
      if (hand[current] > hand[1 - current]) eliminated[1 - current] = true;
      if (hand[current] < hand[1 - current]) eliminated[current] = true;
    }
  } else if (a == HANDMAID) {
    handmaid[current] = true;
  } else if (a == PRINCE) {
    if (hand[1 - current] == PRINCESS) eliminated[1 - current] = true;
    history += std::string(1, hand[1 - current]);
    hand[1 - current] = cards[top];
    top++;
  } else if (a == PRINCE_SELF) {
    if (hand[current] == PRINCESS) eliminated[current] = true;
    history += std::string(1, hand[current]);
    hand[current] = cards[top];
    top++;
  } else if (a == KING) {
    char tmp = hand[current];
    history += std::string(1, hand[current]) + std::string(1, hand[1 - current]);
    hand[current] = hand[1 - current];
    hand[1 - current] = tmp;
  }

  current = 1 - current;
  handmaid[current] = false;
}

}
}
