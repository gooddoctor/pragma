#include <QDebug>
#include <QDir>

#include "game/game.hpp"

#include "object/primitive/base/object.hpp"
#include "object/confirmation.hpp"
#include "object/label.hpp"
#include "object/menu.hpp"
#include "object/spin.hpp"

#include "parser/parser.hpp"

using namespace game;
using namespace object;
using namespace parser;

const int DELAY = 1000 / 100; //fps per second

enum PlayerState {FREE, ACTION, ON_FIRE} player_state = FREE;

//game logic
Game pragma;
//game resources
QDir resource_dir("resource/");
//game graphics
Object* top = nullptr;

void cancel() {
  player_state = FREE;
}

void buy_final(QString resource) {
  Spin* spin = new Spin(top, 0, 400, 25, "spin");
  Confirmation* confirmation = new Confirmation(top, 0, 400 - 60, 20, "confirmation");
  confirmation->on_approved([resource, spin, confirmation]() {
    if (resource == "GAS")
      pragma.bought(GAS, spin->val());
    else if (resource == "OIL")
      pragma.bought(OIL, spin->val());
    else if(resource == "METAL")
      pragma.bought(METAL, spin->val());
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, confirmation));
    top->on_after(cancel);
  });
  confirmation->on_denied([spin, confirmation]() {
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, confirmation));
    top->on_after(cancel);
  });
}

void buy() {
  Menu* menu = new Menu(top, 0, 400, 20, "menu", {"Отмена операции", "GAS", "OIL", "METAL"});
  menu->on_select([](Menu* m, Text* t) {
    if (t->get_text() == "Отмена операции")
      top->on_after(cancel);
    else 
      top->on_after(std::bind(buy_final, t->get_text()));
    //delete current menu
    top->on_after(std::bind(&Object::remove, top, m));
  });
}

void sell_final(QString resource) {
  Spin* spin = new Spin(top, 0, 400, 25, "spin");
  Confirmation* confirmation = new Confirmation(top, 0, 400 - 60, 20, "confirmation");
  confirmation->on_approved([resource, spin, confirmation]() {
    if (resource == "GAS")
      pragma.sold(GAS, spin->val());
    else if (resource == "OIL")
      pragma.sold(OIL, spin->val());
    else if(resource == "METAL")
      pragma.sold(METAL, spin->val());
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, confirmation));
    top->on_after(cancel);
  });
  confirmation->on_denied([spin, confirmation]() {
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, confirmation));
    top->on_after(cancel);
  });
}

void sell() {
  Menu* menu = new Menu(top, 0, 400, 20, "menu", {"Отмена операции", "GAS", "OIL", "METAL"});
  menu->on_select([](Menu* m, Text* t) {
    if (t->get_text() == "Отмена операции")
      top->on_after(cancel);
    else 
      top->on_after(std::bind(sell_final, t->get_text()));
    //delete current menu
    top->on_after(std::bind(&Object::remove, top, m));
  });
}

void kill_final(QString victim) {
  Spin* spin = new Spin(top, 0, 400, 25, "spin");
  Confirmation* confirmation = new Confirmation(top, 0, 400 - 60, 20, "confirmation");
  confirmation->on_approved([victim, spin, confirmation]() {
    if (victim == "A")
      pragma.kill(A, spin->val());
    else if (victim == "B")
      pragma.kill(B, spin->val());
    else if (victim == "C")
      pragma.kill(C, spin->val());
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, confirmation));
    top->on_after(cancel);
  });
  confirmation->on_denied([spin, confirmation]() {
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, confirmation));
    top->on_after(cancel);
  });
}

void kill() {
  Menu* menu = new Menu(top, 0, 400, 20, "menu", {"Отмена операции", "A", "B", "C"});  
  menu->on_select([](Menu* m, Text* t) {
    if (t->get_text() == "Отмена операции")
      top->on_after(cancel);
    else 
      top->on_after(std::bind(kill_final, t->get_text()));
    //delete current menu
    top->on_after(std::bind(&Object::remove, top, m));
  });
}

void rob_final(QString victim) {
  Spin* spin = new Spin(top, 0, 400, 25, "spin");
  Confirmation* confirmation = new Confirmation(top, 0, 400 - 60, 20, "confirmation");
  confirmation->on_approved([victim, spin, confirmation]() {
    if (victim == "A")
      pragma.rob(A, spin->val());
    else if (victim == "B")
      pragma.rob(B, spin->val());
    else if (victim == "C")
      pragma.rob(C, spin->val());
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, confirmation));
    top->on_after(cancel);
  });
  confirmation->on_denied([spin, confirmation]() {
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, confirmation));
    top->on_after(cancel);
  });
}

void rob() {
  Menu* menu = new Menu(top, 0, 400, 20, "menu", {"Отмена операции", "A", "B", "C"});
  menu->on_select([](Menu* m, Text* t) {
    if (t->get_text() == "Отмена операции")
      top->on_after(cancel);
    else 
      top->on_after(std::bind(rob_final, t->get_text()));
    //delete current menu
    top->on_after(std::bind(&Object::remove, top, m));
  });
}

void undead() {
  Label* warning = new Label(top, 0, 280, 20, "warning", "", "you have been: dead. Restrart?");
  Confirmation* confirmation = new Confirmation(top, 0, 400 - 60, 20, "confirmation");
  confirmation->on_approved([warning, confirmation]() {
    top->on_after(std::bind(&Object::remove, top, warning));
    top->on_after(std::bind(&Object::remove, top, confirmation));
    top->on_after(cancel);
  });
}

void unkill() {
  Label* warning = new Label(top, 0, 280, 20, "warning", "", "you have been: killed");
  Spin* spin = new Spin(top, 0, 400, 25, "spin");
  Confirmation* confirmation = new Confirmation(top, 0, 400 - 60, 20, "confirmation");
  confirmation->on_approved([warning, spin, confirmation]() {
    pragma.remove_restriction(KILL, spin->val());
    top->on_after(std::bind(&Object::remove, top, warning));
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, confirmation));
    top->on_after(cancel);
  });
}

void unrob() {
  Label* warning = new Label(top, 0, 280, 20, "warning", "", "you have been: robed");
  Spin* spin = new Spin(top, 0, 400, 25, "spin");
  Confirmation* confirmation = new Confirmation(top, 0, 400 - 60, 20, "confirmation");
  confirmation->on_approved([warning, spin, confirmation]() {
    pragma.remove_restriction(ROB, spin->val());
    top->on_after(std::bind(&Object::remove, top, warning));
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, confirmation));
    top->on_after(cancel);
  });
}


void action() {
  if (player_state == FREE) {
    Menu* menu = new Menu(top, 0, 400, 20, "menu", {"Отмена операции", "Купить", "Продать", 
						    "Убить", "Ограбить"});
    menu->on_select([](Menu* m, Text* t) {
      //show next menu
      if (t->get_text() == "Отмена операции")
	top->on_after(cancel);
      else if (t->get_text() == "Купить")
	top->on_after(buy);
      else if (t->get_text() == "Продать")
	top->on_after(sell);
      else if (t->get_text() == "Убить")
	top->on_after(kill);
      else if (t->get_text() == "Ограбить")
	top->on_after(rob);
      //delete current menu
      top->on_after(std::bind(&Object::remove, top, m));
    });
    player_state = ACTION;
  } else if (player_state == ON_FIRE && pragma.is_on_restriction(ME, DEAD)) {
    undead();
  } else if (player_state == ON_FIRE && pragma.is_on_restriction(ME, KILL)) {
    unkill();
  } else if (player_state == ON_FIRE && pragma.is_on_restriction(ME, ROB)) {
    unrob();
  }
}


int main(int argc, char** argv) {
  //init fonts
  TTF_Init();
  //init players
  std::map<PLAYER, Player> players{{A, Player()}, {B, Player()}, {C, Player()}};
  //tmx part
  TMX tmx = TMXParser::comming_in_fast().parse("resource/map.tmx");
  int width = std::get<Map>(tmx)["width"] * std::get<Map>(tmx)["tilewidth"];
  int height = std::get<Map>(tmx)["height"] * std::get<Map>(tmx)["tileheight"] + 64; //extra space
  //sdl part
  SDL_Window* window = SDL_CreateWindow("pragma", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
					width, height, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);  
  //create top object 
  top = new Object(nullptr, -1, -1, -1, "TOP");
  //add tmx objects
  for (auto it : LayerParser::comming_in_fast().parse(tmx))
    new Image(top, std::get<X>(it), std::get<Y>(it), std::get<Z>(it), "", 
	      resource_dir.filePath(std::get<Source>(it)));  
  //add resource labels;
  Label* gas = new Label(top, 0, 16, 20, "gas", resource_dir.filePath("gas.png"), " ");
  Label* oil = new Label(top, 100, 16, 20, "oil", resource_dir.filePath("oil.png"), " ");
  Label* metal = new Label(top, 200, 16, 20, "metal", resource_dir.filePath("metal.png"), " ");
  Label* money = new Label(top, 300, 16, 20, "money", resource_dir.filePath("money.png"), " ");  
  //add players images 
  Image* ego = new Image(top, 100, 380, 20, "ego", resource_dir.filePath("alter_ego.png"));
  Image* a_player = new Image(top, 0, 464, 25, "a_player", resource_dir.filePath("a.png"));
  Image* b_player = new Image(top, 0 + 64, 464, 25, "b_player", resource_dir.filePath("b.png"));
  Image* c_player = new Image(top, 0 + 64 + 64, 464, 25, "c_player", resource_dir.filePath("c.png"));
  Image* me = new Image(top, 0 + 64 + 64 + 64, 464, 25, "me", resource_dir.filePath("me.png"));
  //next move image
  Image* hourglass = new Image(top, 400 - 64, 464, 20, "Hourglass", 
			       resource_dir.filePath("hourglass.png"));
  //callbacks part
  pragma.on_trade([gas, oil, metal, money]() {
    gas->set_text(QString::number(pragma.get_player_resource(ME)[GAS])),
    oil->set_text(QString::number(pragma.get_player_resource(ME)[OIL]));
    metal->set_text(QString::number(pragma.get_player_resource(ME)[METAL]));
    money->set_text(QString::number(pragma.get_player_resource(ME)[MONEY]));
  });
  ego->on_mouse_button_up([](Sprite*, const SDL_Event&) {
    top->on_after(&action);
  });
  hourglass->on_mouse_button_up([](Sprite*, const SDL_Event&) {
    pragma.made_move();
  });  
  pragma.on_made_move([a_player, b_player, c_player, me]() {
    a_player->deselect();
    b_player->deselect();
    c_player->deselect(),
    me->deselect();  
    switch (pragma.get_active_player()) {
      case A:
	a_player->select();
  	break;
      case B:
	b_player->select();
	break;
      case C:
	c_player->select();
	break;
      case ME:
	me->select();
    }
  });    
  
  //main cycle 
  bool is_running = true;
  while (is_running) {
    Uint32 start = SDL_GetTicks();
    //handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch(event.type) {
	case SDL_QUIT:
	  is_running = false;
	  break;
	/* MANY
	 * MORE
	 * CASES: 
	 * perhaps
	*/
	default:
	  top->event(event);
	  top->after();
	  break;
      }
    }
    //update
    if (pragma.get_active_player() != ME)
      players[pragma.get_active_player()].make_move(pragma);
    if (pragma.is_on_restriction(ME, DEAD) ||
	pragma.is_on_restriction(ME, KILL) || 
	pragma.is_on_restriction(ME, ROB))
      player_state = ON_FIRE;
    //render
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
    top->render(renderer);
    SDL_RenderPresent(renderer);
    //sleep a litle bit
    Uint32 duration = SDL_GetTicks() - start;
    if (duration < DELAY)
      SDL_Delay((int)(DELAY - duration));
  }
}




