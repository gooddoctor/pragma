#ifndef __GAME_H
#define __GAME_H

#include <map>

#include <QString>

namespace game {
  enum RESOURCE {GAS, OIL, METAL, MONEY};
  static const QString RESOURCE_to_str[] = {"GAS", "OIL", "METAL", "MONEY"};

  enum PLAYER {A, B, C, ME};
  static const QString PLAYER_to_str[] = {"A", "B", "C", "ME"};

  enum RESTRICTION {DEAD, KILL, ROB};
  static const QString RESTRICTION_to_str[] = {"DEAD", "KILL", "ROB"};

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
    int get_resource(RESOURCE x);
    Resource get_player_resource(PLAYER player);
    int get_player_resource(RESOURCE x);
    Game* bought(RESOURCE x, int amount);
    Game* sold(RESOURCE x, int amount);
    Game* kill(PLAYER victim, int amount);
    Game* rob(PLAYER victim, int amount);
    bool remove_restriction(RESTRICTION restriction, int amount);
    bool remove_kill_restriction(int amount);
    bool remove_rob_restriction(int amount);
    bool is_on_restriction(PLAYER player, RESTRICTION restriction);
    bool is_on_restriction(RESTRICTION restriction);
    Game* made_move();
    Game* reset();
    Game* on_remove_restriction(const Callback& callback);
    Game* on_made_move(const Callback& callback);
    Game* on_trade(const Callback& callback);
    Game* on_reset(const Callback& callback);
    QString to_string();
  private:
    Game* fire_callbacks(const std::vector<Callback>& callbacks);
    Game* trade(RESOURCE x, int x_amount, RESOURCE y, int y_amount);
  private:
    PLAYER active_player = ME;
    int total_moves = 0;
    Resource resource;
    PlayersResource players_resource;
    PlayersRestriction players_restriction;
    Turn players_turn;
    std::vector<Callback> remove_restriction_callbacks;
    std::vector<Callback> made_move_callbacks;
    std::vector<Callback> trade_callbacks;
    std::vector<Callback> reset_callbacks;
  };

  class Player {
  public:
    Player* make_move(game::Game& game);
  private:
    bool think_enough(Game&);
    bool remove_restriction(Game& game);
    bool action(Game& game);
    bool change_state(Game&);    
  private:
    enum State {BOUGHT, SOLD, MURDER, ROBERRY} state = BOUGHT;
    int think_time = 0;
  };
}

#endif
