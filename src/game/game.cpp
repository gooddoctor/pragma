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

Game* Game::player_bought(RESOURCE x, int amount) {
  return player_trade(MONEY, resource[x] * amount, x, amount);
}

Game* Game::player_sold(RESOURCE x, int amount) {
  return player_trade(x, amount, MONEY, resource[x] * amount);
}

Game* Game::player_killed(PLAYER victim, int amount) {
  players_restriction[victim].insert({KILL, amount});
  return this;
}

bool Game::player_remove_kill_restriction(int amount) {
  auto it = players_restriction[get_active_player()].find(KILL);
  int blood_money = it->second;
  if (amount > blood_money) {
    players_restriction[get_active_player()].erase(it);
    player_trade(MONEY, amount, MONEY, 0); // MONEY - AMOUNT : MONEY + 0
    return true;
  }
  return false;
}

bool Game::is_player_on_restriction(PLAYER player, RESTRICTION restriction) {
  return players_restriction[player].find(restriction) != players_restriction[player].end();
}

Game* Game::player_made_move() {
  active_player = players_turn[active_player];
  //notify about move
  for (auto it : player_made_move_callbacks) it();
  //if last player is moved let economic works
  total_moves++;
  if ((total_moves %4) == 0)
    for (auto& it : resource) it.second = get_new_price(it.first, it.second);
  return this;
}

Game* Game::on_player_trade(const Callback& callback) {
  player_trade_callbacks.push_back(callback);
  return this;
}

Game* Game::on_player_made_move(const Callback& callback) {
  player_made_move_callbacks.push_back(callback);
  return this;
}

QString Game::to_string() {
  QString str;
  for (auto it : players_resource)
    str.append(QString("%1:%2\n").arg(PLAYER_to_str[it.first]).arg(resource_to_string(it.second)));
  return str;
}

Game* Game::player_trade(RESOURCE x, int x_amount, RESOURCE y, int y_amount) {
  //trade and notify about it
  players_resource[active_player][x] -= x_amount;
  players_resource[active_player][y] += y_amount;
  for (auto it : player_trade_callbacks) it();
  return this;
}

Player* Player::make_move(game::Game& game) {
  //think a little bit
  if (!is_think_enough())
    return this;
  //if we got killed
  if (game.is_player_on_restriction(game.get_active_player(), KILL)) {
    game.player_remove_kill_restriction(game.get_player_resource(game.get_active_player())[MONEY] - 1);
    return this;
  }
  //make move
  switch (state) {
    case BOUGHT: {
      int money = game.get_player_resource(game.get_active_player())[MONEY];
      int gas_price = game.get_resource()[GAS];
      int gas_amount = money / gas_price;
      game.player_bought(GAS, gas_amount);
      state = SOLD;
      break;
    } case SOLD: {
      int gas_amount = game.get_player_resource(game.get_active_player())[GAS];
      game.player_sold(GAS, gas_amount);
      state = BOUGHT;
      break;
    }
  }
  game.player_made_move();
  return this;
}

bool Player::is_think_enough() {
  return ((++think_time % 60) == 0) ? true : false;
} 
