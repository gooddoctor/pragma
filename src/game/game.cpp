#include <QDebug>

#include "game.hpp"

using namespace game;

QString resource_to_string(Resource resource) {
  QString str;
  for (auto it : resource)
    str.append(QString("(%1:%2)").arg(RESOURCE_to_str[it.first]).arg(it.second));
  return str;
}

double get_new_profit(RESOURCE, double) {
  return (rand() % (4000 + 1) + 1000) / 1000.0; //from 1.0 to 5.0
}

int get_new_price(RESOURCE, int current_price) {
  return current_price;
}

int random_piece(int entire, int a, int b) {
  double rnd = ((rand() % (b - a + 1)) + a) / 100.0;
  return entire * rnd;
}

Game::Game() {
  reset();
}

Game* Game::exit() {
  is_start = false;
  return this;
}

bool Game::is_exit() {
  return !is_start;
}

PLAYER Game::get_active_player() {
  return active_player;
}

Resource Game::get_resource() {
  return resource;
}

int Game::get_resource(RESOURCE x) {
  return resource[x];
}

Resource Game::get_player_resource(PLAYER player) {
  return players_resource[player];
}

int Game::get_player_resource(RESOURCE x) {
  return get_player_resource(active_player)[x];
}

Game* Game::bought(RESOURCE x, int amount) {
  return trade(MONEY, resource[x] * amount, x, amount);
}

Game* Game::sold(RESOURCE x, int amount) {
  return trade(x, amount, MONEY, resource[x] * amount * profit[x]);
}

Game* Game::kill(PLAYER victim, int amount) {
  players_restriction[victim].insert({KILL, amount});
  return trade(MONEY, amount, MONEY, 0);
}

Game* Game::rob(PLAYER victim, int amount) {
  players_restriction[victim].insert({ROB, amount});
  return trade(MONEY, amount, MONEY, 0);
}

bool Game::remove_restriction(RESTRICTION restriction, int amount) {
  bool result = false;
  switch (restriction) {
  case DEAD:
    result = false;
    break;
  case KILL:
    result = remove_kill_restriction(amount);
    break;
  case ROB:
    result = remove_rob_restriction(amount);
    break;
  }
  fire_callbacks(remove_restriction_callbacks);
  return result;
}

bool Game::remove_kill_restriction(int amount) {
  auto it = players_restriction[active_player].find(KILL);
  int blood_money = it->second;
  if (amount > blood_money) {
    players_restriction[active_player].erase(it);
    trade(MONEY, amount, MONEY, 0); // MONEY - AMOUNT
    return true;
  }
  players_restriction[active_player].insert({DEAD, 0});
  fire_callbacks(dead_callbacks);
  return false;
}

bool Game::remove_rob_restriction(int amount) {
  auto it = players_restriction[active_player].find(ROB);
  int blood_money = it->second;
  players_restriction[active_player].erase(it);
  trade(MONEY, std::abs(blood_money - amount), MONEY, 0); //MONEY - delta
  return true;
}

bool Game::is_on_restriction(PLAYER player, RESTRICTION restriction) {
  return players_restriction[player].find(restriction) != players_restriction[player].end();
}

bool Game::is_on_restriction(RESTRICTION restriction) {
  return is_on_restriction(active_player, restriction);
}

Game* Game::made_move() {
  active_player = players_turn[active_player];
  fire_callbacks(made_move_callbacks);
  //if last player is moved let economic works
  total_moves++;
  if ((total_moves % 4) == 0) {
    //generating new price and profit structures
    for (auto& it : profit) it.second = get_new_profit(it.first, it.second);
    for (auto& it : resource) it.second = get_new_price(it.first, it.second);
  }
  return this;
}

Game* Game::reset() {
  players_resource.clear();
  resource.clear();
  players_turn.clear();
  players_restriction.clear();
  //init profit
  profit.insert({GAS, 1.5});
  profit.insert({OIL, 1.5});
  profit.insert({METAL, 1.5});
  //init players_resource 
  players_resource.insert({A, Resource{{GAS, 0}, {OIL, 0}, {METAL, 0}, {MONEY, 10}}});
  players_resource.insert({B, Resource{{GAS, 0}, {OIL, 0}, {METAL, 0}, {MONEY, 10}}});
  players_resource.insert({C, Resource{{GAS, 0}, {OIL, 0}, {METAL, 0}, {MONEY, 10}}});
  players_resource.insert({ME, Resource{{GAS, 0}, {OIL, 0}, {METAL, 0}, {MONEY, 10}}});
  //init resource
  resource.insert({GAS, 1});
  resource.insert({OIL, 1});
  resource.insert({METAL, 1});
  //init players turn
  players_turn.insert({ME, A});
  players_turn.insert({A, B});
  players_turn.insert({B, C});
  players_turn.insert({C, ME});
  //init restrictions
  players_restriction.insert({A, Restriction{}});
  players_restriction.insert({B, Restriction{}});
  players_restriction.insert({C, Restriction{}});
  players_restriction.insert({ME, Restriction{}});

  fire_callbacks(reset_callbacks);

  return this;
}

Game* Game::on_dead(const Callback& callback) {
  dead_callbacks.push_back(callback);
  return this;
}

Game* Game::on_remove_restriction(const Callback& callback) {
  remove_restriction_callbacks.push_back(callback);
  return this;
}

Game* Game::on_made_move(const Callback& callback) {
  made_move_callbacks.push_back(callback);
  return this;
}

Game* Game::on_trade(const Callback& callback) {
  trade_callbacks.push_back(callback);
  return this;
}

Game* Game::on_reset(const Callback& callback) {
  reset_callbacks.push_back(callback);
  return this;
}

QString Game::to_string() {
  QString str;
  for (auto it : players_resource)
    str.append(QString("%1:%2\n").arg(PLAYER_to_str[it.first]).arg(resource_to_string(it.second)));
  return str;
}

Game* Game::fire_callbacks(const std::vector<Callback>& callbacks) {
  for (auto it : callbacks) it();
  return this;
}

Game* Game::trade(RESOURCE x, int x_amount, RESOURCE y, int y_amount) {
  if (players_resource[active_player][x] >= x_amount) {
    //trade and notify about it
    players_resource[active_player][x] -= x_amount;
    players_resource[active_player][y] += y_amount;
    fire_callbacks(trade_callbacks);
  }
  return this;
}

Player* Player::make_move(game::Game& game) {
  if (!think_enough(game))
    return this;
  qDebug() << "before " << game.to_string();
  if (!remove_restriction(game)) {
    game.made_move();
    return this;
  }
  if (!action(game)) {
    game.made_move();
    return this;
  }
  if (!change_state(game)) {
    game.made_move();
    return this;
  }
  qDebug() << "after " << game.to_string();
  game.made_move();
  return this;
}

Player* Player::once_more_with_feelings() {
  state = BOUGHT;
  return this;
}

bool Player::think_enough(Game&) {
  return ((++think_time % 60) == 0) ? true : false;
}

bool Player::remove_restriction(Game& game) {
  if (game.is_on_restriction(DEAD)) {
    return false;
  } else if (game.is_on_restriction(KILL)) {
    return game.remove_kill_restriction(random_piece(game.get_player_resource(MONEY), 30, 50));
  } else if (game.is_on_restriction(ROB)) {
    return game.remove_rob_restriction(random_piece(game.get_player_resource(MONEY), 10, 30));
  } else {
    return true;
  }
}

bool Player::action(Game& game) {
  switch (state) {
    case BOUGHT: {
      RESOURCE victim = static_cast<RESOURCE>((int)rand() % RESOURCE::RESOURCE_SIZE);
      while (victim == MONEY) //i'm sory. we cant buy money. 
	victim = static_cast<RESOURCE>((int)rand() % RESOURCE::RESOURCE_SIZE);
      game.bought(victim, game.get_player_resource(MONEY) / game.get_resource(victim));
      break;
    } case SOLD:
      game.sold(GAS, game.get_player_resource(GAS));
      game.sold(OIL, game.get_player_resource(OIL));
      game.sold(METAL, game.get_player_resource(METAL));
      break;
    case MURDER: {
      //get our victim
      PLAYER victim = static_cast<PLAYER>((int)rand() % PLAYER::PLAYER_SIZE);
      while (victim == game.get_active_player() || game.is_on_restriction(victim, DEAD))
	victim = static_cast<PLAYER>((int)rand() % PLAYER::PLAYER_SIZE);
      //kill it now
      game.kill(victim, random_piece(game.get_player_resource(MONEY) - 10, 70, 90));
      break;
    } 
  }
  return true;
}

bool Player::change_state(Game& game) {
  switch (state) {
    case BOUGHT: 
	state = SOLD;
      break;
    case SOLD:
      if (game.get_player_resource(MONEY) >= 15)
	state = MURDER;
      else
	state = BOUGHT;
      break;
    case MURDER:
      state = BOUGHT;
      break;
  }
  return true;
}
    
