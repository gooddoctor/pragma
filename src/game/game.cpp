#include <QDebug>

#include "game.hpp"

using namespace game;

QString resource_to_string(Resource resource) {
  QString str;
  for (auto it : resource)
    str.append(QString("(%1:%2)").arg(RESOURCE_to_str[it.first]).arg(it.second));
  return str;
}

int get_new_price(RESOURCE x, int current_price) {
  return current_price + 2;
}

int random_piece(int entire, int a, int b) {
  double rnd = ((rand() % (b - a + 1)) + a) / 100.0;
  return entire * rnd;
}

Game::Game() {
  //init players_resource
  players_resource.insert({A, Resource{{GAS, 0}, {OIL, 0}, {METAL, 0}, {MONEY, 100}}});
  players_resource.insert({B, Resource{{GAS, 0}, {OIL, 0}, {METAL, 0}, {MONEY, 100}}});
  players_resource.insert({C, Resource{{GAS, 0}, {OIL, 0}, {METAL, 0}, {MONEY, 100}}});
  players_resource.insert({ME, Resource{{GAS, 0}, {OIL, 0}, {METAL, 0}, {MONEY, 100}}});
  //init resource
  resource.insert({GAS, 5});
  resource.insert({OIL, 6});
  resource.insert({METAL, 7});
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
}

PLAYER Game::get_active_player() {
  return active_player;
}

Resource Game::get_resource() {
  return resource;
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
  return trade(x, amount, MONEY, resource[x] * amount);
}

Game* Game::kill(PLAYER victim, int amount) {
  players_restriction[victim].insert({KILL, amount});
  return this;
}

Game* Game::rob(PLAYER victim, int amount) {
  players_restriction[victim].insert({ROB, amount});
  return this;
}

bool Game::remove_kill_restriction(int amount) {
  auto it = players_restriction[active_player].find(KILL);
  int blood_money = it->second;
  if (amount > blood_money) {
    players_restriction[active_player].erase(it);
    trade(MONEY, amount, MONEY, 0); // MONEY - AMOUNT
    return true;
  }
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
  //notify about move
  for (auto it : made_move_callbacks) it();
  //if last player is moved let economic works
  total_moves++;
  if ((total_moves %4) == 0)
    for (auto& it : resource) it.second = get_new_price(it.first, it.second);
  return this;
}

Game* Game::on_trade(const Callback& callback) {
  trade_callbacks.push_back(callback);
  return this;
}

Game* Game::on_made_move(const Callback& callback) {
  made_move_callbacks.push_back(callback);
  return this;
}

QString Game::to_string() {
  QString str;
  for (auto it : players_resource)
    str.append(QString("%1:%2\n").arg(PLAYER_to_str[it.first]).arg(resource_to_string(it.second)));
  return str;
}

Game* Game::trade(RESOURCE x, int x_amount, RESOURCE y, int y_amount) {
  //trade and notify about it
  players_resource[active_player][x] -= x_amount;
  players_resource[active_player][y] += y_amount;
  for (auto it : trade_callbacks) it();
  return this;
}

Player* Player::make_move(game::Game& game) {
  //think a little bit
  if (!is_think_enough())
    return this;
  //show info
  qDebug() << "before " << PLAYER_to_str[game.get_active_player()] << ":" 
	   << game.get_player_resource(MONEY);
  //if we got killed
  if (game.is_on_restriction(KILL)) {
    game.remove_kill_restriction(random_piece(game.get_player_resource(MONEY), 30, 50));
    return this;
  }
  if (game.is_on_restriction(ROB)) {
    game.remove_rob_restriction(random_piece(game.get_player_resource(MONEY), 10, 30));
    return this;
  }
  //make move
  switch (state) {
    case BOUGHT: {
      int money = random_piece(game.get_player_resource(MONEY), 70, 70); //spend only 70%
      int gas_price = game.get_resource()[GAS];
      int gas_amount = money / gas_price;
      game.bought(GAS, gas_amount);
      state = SOLD;
      break;
    } case SOLD: {
      int gas_amount = game.get_player_resource(GAS);
      game.sold(GAS, gas_amount);
      state = BOUGHT;
      break;
    }
  }
  qDebug() << "after " << PLAYER_to_str[game.get_active_player()] << ":" 
	   << game.get_player_resource(MONEY);
  game.made_move();
  return this;
}

bool Player::is_think_enough() {
  return ((++think_time % 60) == 0) ? true : false;
} 
