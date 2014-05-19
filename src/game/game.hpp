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
  typedef std::map<PLAYER, PLAYER> After;
  public:
    Game();
    Game* player_bought(RESOURCE resource, int amount);
    Game* player_sold(RESOURCE resource, int amount);
    Game* player_made_move();
    QString to_string();
  private:
    Game* player_trade(RESOURCE x, int x_amount, RESOURCE y, int y_amount);
  private:
    Resources resources;
    Players players;
    After after;
    PLAYER current_one = ME;
  };
}

#endif
