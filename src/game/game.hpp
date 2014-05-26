#ifndef __GAME_H
#define __GAME_H

#include <map>

#include <QString>

namespace game {
  enum RESOURCE {GAS, OIL, METAL, MONEY};
  static const QString RESOURCE_to_str[] = {"GAS", "OIL", "METAL", "MONEY"};

  enum PLAYER {A, B, C, ME};
  static const QString PLAYER_to_str[] = {"A", "B", "C", "ME"};

  typedef std::map<RESOURCE, int> Resources;
  typedef std::map<PLAYER, Resources> Players;

  class Game {
  typedef std::function<void(void)> Callback;
  typedef std::map<PLAYER, PLAYER> Turn;
  public:
    Game();
    PLAYER get_active_player();
    Game* player_bought(RESOURCE resource, int amount);
    Game* player_sold(RESOURCE resource, int amount);
    Game* player_made_move();
    Game* on_player_made_move(const Callback& callback);
    QString to_string();
  private:
    Game* player_trade(RESOURCE x, int x_amount, RESOURCE y, int y_amount);
  private:
    PLAYER active_player = ME;
    Resources resources;
    Players players;
    Turn players_turn;
    std::vector<Callback> player_made_move_callbacks;
  };

  class Player {
  public:
    Player* make_move(game::Game& game);
  };
}

#endif
