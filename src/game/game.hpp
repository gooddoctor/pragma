#ifndef __GAME_H
#define __GAME_H

#include <map>

#include <QString>

namespace game {
  enum RESOURCE {GAS, OIL, METAL, MONEY};
  static const QString RESOURCE_to_str[] = {"GAS", "OIL", "METAL", "MONEY"};

  enum PLAYER {A, B, C, ME};
  static const QString PLAYER_to_str[] = {"A", "B", "C", "ME"};

  enum RESTRICTION {KILL, ROB};
  static const QString RESTRICTION_to_str[] = {"KILL", "ROB"};

  typedef std::map<RESOURCE, int> Resource;
  typedef std::map<PLAYER, Resource> PlayersResource;

  typedef std::multimap<RESTRICTION, int> Restriction;
  typedef std::map<PLAYER, Restriction> PlayersRestriction;

  class Game {
  typedef std::function<void(void)> Callback;
  typedef std::map<PLAYER, PLAYER> Turn;
  public:
    Game();
    PLAYER get_active_player();
    Resource get_resource();
    Resource get_player_resource(PLAYER player);
    int get_player_resource(RESOURCE resource);
    Game* bought(RESOURCE resource, int amount);
    Game* sold(RESOURCE resource, int amount);
    Game* kill(PLAYER victim, int amount);
    Game* rob(PLAYER victim, int amount);
    bool remove_kill_restriction(int amount);
    bool remove_rob_restriction(int amount);
    bool is_on_restriction(PLAYER player, RESTRICTION restriction);
    bool is_on_restriction(RESTRICTION restriction);
    Game* made_move();
    Game* on_made_move(const Callback& callback);
    Game* on_trade(const Callback& callback);
    QString to_string();
  private:
    Game* trade(RESOURCE x, int x_amount, RESOURCE y, int y_amount);
  private:
    PLAYER active_player = ME;
    int total_moves = 0;
    Resource resource;
    PlayersResource players_resource;
    PlayersRestriction players_restriction;
    Turn players_turn;
    std::vector<Callback> made_move_callbacks;
    std::vector<Callback> trade_callbacks;
  };

  class Player {
  public:
    Player* make_move(game::Game& game);
  private:
    bool is_think_enough();
  private:
    enum State {BOUGHT, SOLD} state = BOUGHT;
    int think_time = 0;
  };
}

#endif
