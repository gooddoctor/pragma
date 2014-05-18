#ifndef __GAME_H
#define __GAME_H

#include <map>

#include <QString>

namespace game {
  enum RESOURCE {GAS, OIL, METAL, MONEY};
  enum PLAYER {A, B, C, ME};
  typedef std::map<RESOURCE, int> Resources;
  typedef std::map<PLAYER, Resources> Players;
  class Game {
  public:
    Game();
    Game* player_buying(RESOURCE resource, int amount);
    Game* player_selling(RESOURCE resource, int amount);
    Game* player_made_move();
    QString to_string();
  private:
    Game* player_trade(RESOURCE x, int x_amount, RESOURCE y, int y_amount);
  private:
    Resources resources;
    Players players;
    PLAYER one = ME;
  };
}

#endif
