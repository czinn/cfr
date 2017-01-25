#ifndef KUHN_GAME_H
#define KUHN_GAME_H

#define CARDS 9

#include "Game.h"

class KuhnGame : public Game {
public:
  KuhnGame(long seed);
  KuhnGame(const KuhnGame &game);
  virtual ~KuhnGame();

  virtual KuhnGame *clone() const override;
  virtual int currentPlayer() const override;
  virtual bool isTerminal() const override;
  virtual double utility() const override;
  virtual std::string getInformationSetId() const override;
  virtual int numActions() const override;

  virtual void makeAction(int action) override;

private:
  int cards[CARDS];
  std::string history;
  int current;
};

#endif
