#ifndef GAME_H
#define GAME_H

#include <string>

namespace cfr {

class Game {
public:
  Game(long seed);
  Game(const Game &game);
  virtual ~Game();

  virtual Game *clone() const = 0;
  virtual int currentPlayer() const = 0;
  virtual bool isTerminal() const = 0;
  virtual double utility() const = 0;
  virtual std::string getInformationSetId() const = 0;
  virtual int numActions() const = 0;

  virtual void makeAction(int action) = 0;
};

}

#endif
