#ifndef LOVE_LETTER_GAME_H
#define LOVE_LETTER_GAME_H

#include "../Game.h"

namespace cfr {
namespace love {

const char GUARD = 1;
const char PRIEST = 2;
const char BARON = 3;
const char HANDMAID = 4;
const char PRINCE = 5;
const char KING = 6;
const char COUNTESS = 7;
const char PRINCESS = 8;
const char GUESS = 7; // GUESS + <CARD> is a guess of that card
const char PRINCE_SELF = 16;

class LoveLetterGame : public Game {
public:
  LoveLetterGame(long seed);
  LoveLetterGame(const LoveLetterGame &game);
  virtual ~LoveLetterGame();

  virtual LoveLetterGame *clone() const override;

  virtual int currentPlayer() const override;
  virtual bool isTerminal() const override;
  virtual double utility() const override;
  virtual std::string getInformationSetId() const override;
  virtual int numActions() const override;

  virtual void makeAction(int action) override;

private:
  std::string currentHand() const;
  std::string listActions() const;

  char cards[16];
  int remaining[8]; // number of cards remaining of each type
  int top; // top of deck
  char lastAction;
  int discardScore[2];
  char hand[2];
  bool eliminated[2];
  bool handmaid; // whther the other player is handmaided
  bool known[2]; // whether that player's card is known by the other player
  int current; // current player
};

}
}

#endif
