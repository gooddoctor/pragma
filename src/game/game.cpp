#include "game.hpp"

using namespace game;

QString resources_to_string(Resources resources) {
  QString str;
  for (auto it : resources)
    str.append(QString("(%1:%2)").arg(RESOURCE_to_str[it.first]).arg(it.second));
  return str;
}

Game::Game() {
  //init players
  players.insert({A, Resources{{GAS, 0}, {OIL, 0}, {METAL, 0}, {MONEY, 100}}});
  players.insert({B, Resources{{GAS, 0}, {OIL, 0}, {METAL, 0}, {MONEY, 100}}});
  players.insert({C, Resources{{GAS, 0}, {OIL, 0}, {METAL, 0}, {MONEY, 100}}});
  players.insert({ME, Resources{{GAS, 0}, {OIL, 0}, {METAL, 0}, {MONEY, 100}}});
  //init resources
  resources.insert({GAS, 5});
  resources.insert({OIL, 6});
  resources.insert({METAL, 7});
  //init players turn
  players_turn.insert({ME, A});
  players_turn.insert({A, B});
  players_turn.insert({B, C});
  players_turn.insert({C, ME});
}

PLAYER Game::get_active_player() {
  return active_player;
}

Game* Game::player_bought(RESOURCE resource, int amount) {
  return player_trade(MONEY, resources[resource] * amount, resource, amount);
}

Game* Game::player_sold(RESOURCE resource, int amount) {
  return player_trade(resource, amount, MONEY, resources[resource] * amount);
}

Game* Game::player_made_move() {
  active_player = players_turn[active_player];
  for (auto it : player_made_move_callbacks) it();
  return this;
}

Game* Game::on_player_made_move(const Callback& callback) {
  player_made_move_callbacks.push_back(callback);
  return this;
}

QString Game::to_string() {
  QString str;
  for (auto it : players)
    str.append(QString("%1:%2\n").arg(PLAYER_to_str[it.first]).arg(resources_to_string(it.second)));
  return str;
}

Game* Game::player_trade(RESOURCE x, int x_amount, RESOURCE y, int y_amount) {
  players[active_player][x] -= x_amount;
  players[active_player][y] += y_amount;
  return this;
}

Player* Player::make_move(game::Game& game) {
  game.player_bought(RESOURCE::GAS, 5);
  game.player_made_move();
  return this;
}
