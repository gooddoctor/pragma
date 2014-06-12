#include <QDebug>
#include <QDir>

#include "game/game.hpp"
#include "object/image.hpp"
#include "object/text.hpp"
#include "object/menu.hpp"
#include "object/spin.hpp"
#include "parser/parser.hpp"

using namespace game;
using namespace object;
using namespace parser;

const int DELAY = 1000 / 100; //fps per second

//global object
Game pragma;
QDir resource_dir("resource/");
std::map<PLAYER, Player> players{{A, Player()}, {B, Player()}, {C, Player()}};
Object* top = 0;

Text* gas = 0;
Text* oil = 0;
Text* metal = 0;
Text* money = 0;

Image* user_choice = 0;
Image* selected = 0;

//global variable
bool is_running = true;

void count(QString text, std::function<void(RESOURCE, int)> action) {
  Spin* spin = new Spin(top, 0, 400, 25, "count");
  Image* approved = new Image(top, 0, 400 - 60, 20, "approved", 
			      resource_dir.filePath("approved.png"));
  Image* denied = new Image(top, 64 + 2, 400 - 60, 20, "denied", 
			    resource_dir.filePath("denied.png"));
  approved->on_mouse_button_up([spin, approved, denied, text, action](Sprite*, const SDL_Event&) {
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, approved));
    top->on_after(std::bind(&Object::remove, top, denied));
    if (text == "GAS")
      action(GAS, spin->val());
    else if (text == "OIL")
      action(OIL, spin->val());
    else if (text == "METAL")
      action(METAL, spin->val());
    user_choice->hide();
  });
  approved->on_mouse_motion([](Sprite* s, const SDL_Event&) {
    user_choice->show();
    user_choice->set_x(s->get_x() + (s->w() - user_choice->w()) / 2);
    user_choice->set_y(s->get_y() - s->h());
  });
  denied->on_mouse_button_up([spin, approved, denied](Sprite*, const SDL_Event) {
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, approved));
    top->on_after(std::bind(&Object::remove, top, denied));
    user_choice->hide();
  });
  denied->on_mouse_motion([](Sprite* s, const SDL_Event&) {
    user_choice->show();
    user_choice->set_x(s->get_x() + (s->w() - user_choice->w()) / 2);
    user_choice->set_y(s->get_y() - s->h());
  });
}

void who(int amount) {
  Menu* menu = new Menu(top, 0, 400, 20, "Buy", {"ОТМЕНА ОПЕРАЦИИ", "A", "B", "C"});
  menu->on_select([amount](Menu* m, Text* t) {
    top->on_after(std::bind(&Object::remove, top, m));
    if (t->get_text() == "A")
      pragma.kill(A, amount);
    else if (t->get_text() == "B")
      pragma.kill(B, amount);
    else if (t->get_text() == "C")
      pragma.kill(C, amount);
  });
}

void who_2(int amount) {
  Menu* menu = new Menu(top, 0, 400, 20, "ROB", {"ОТМЕНА ОПЕРАЦИИ", "A", "B", "C"});
  menu->on_select([amount](Menu* m, Text* t) {
    top->on_after(std::bind(&Object::remove, top, m));
    if (t->get_text() == "A")
      pragma.rob(A, amount);
    else if (t->get_text() == "B")
      pragma.rob(B, amount);
    else if (t->get_text() == "C")
      pragma.rob(C, amount);
  });
}

void kill() {
  Spin* spin = new Spin(top, 0, 400, 25, "count");
  Image* approved = new Image(top, 0, 400 - 60, 20, "approved", 
			      resource_dir.filePath("approved.png"));
  Image* denied = new Image(top, 64 + 2, 400 - 60, 20, "denied", 
			    resource_dir.filePath("denied.png"));
  approved->on_mouse_button_up([spin, approved, denied](Sprite*, const SDL_Event&) {
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, approved));
    top->on_after(std::bind(&Object::remove, top, denied));
    top->on_after(std::bind(who, spin->val()));
  });
}


void rob() {
  Spin* spin = new Spin(top, 0, 400, 25, "count");
  Image* approved = new Image(top, 0, 400 - 60, 20, "approved", 
			      resource_dir.filePath("approved.png"));
  Image* denied = new Image(top, 64 + 2, 400 - 60, 20, "denied", 
			    resource_dir.filePath("denied.png"));
  approved->on_mouse_button_up([spin, approved, denied](Sprite*, const SDL_Event&) {
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, approved));
    top->on_after(std::bind(&Object::remove, top, denied));
      top->on_after(std::bind(who_2, spin->val()));
    });
  }

void buy() {
  Menu* menu = new Menu(top, 0, 400, 20, "Buy", {"ОТМЕНА ОПЕРАЦИИ", "GAS", "OIL", "METAL"});
  menu->on_select([](Menu* m, Text* t) {
    top->on_after(std::bind(&Object::remove, top, m));
    if (t->get_text() != "ОТМЕНА ОПЕРАЦИИ")
      top->on_after(std::bind(count, t->get_text(), [](RESOURCE r, int a) {
	pragma.bought(r, a);
      }));
  });
}

void sell() {
  Menu* menu = new Menu(top, 0, 400, 20, "Sell", {"ОТМЕНА ОПЕРАЦИИ", "GAS", "OIL", "METAL"});
  menu->on_select([](Menu* m, Text* t) {
    top->on_after(std::bind(&Object::remove, top, m));
    if (t->get_text() != "ОТМЕНА ОПЕРАЦИИ")
      top->on_after(std::bind(count, t->get_text(), [](RESOURCE r, int a) {
	pragma.sold(r, a);
      }));
  });
}

void trade() {
  Menu* menu = new Menu(top, 0, 400, 20, "trade", {"ОТМЕНА ОПЕРАЦИИ", "Продать", "Купить",
						   "УБИТЬ", "ОГРАБИТЬ"});
  menu->on_select([](Menu* m, Text* t) { 
    top->on_after(std::bind(&Object::remove, top, m));
    if (t->get_text() == "Купить")
      top->on_after(buy);
    else if (t->get_text() == "Продать")
      top->on_after(sell);
    else if (t->get_text() == "УБИТЬ")
      top->on_after(kill);
    else if (t->get_text() == "ОГРАБИТЬ")
      top->on_after(rob);
  });
}

int main(int, char**) {
  //init part
  TTF_Init();

  //tmx part
  TMX tmx = TMXParser::comming_in_fast().parse("resource/map.tmx");
  int width = std::get<Map>(tmx)["width"] * std::get<Map>(tmx)["tilewidth"];
  int height = std::get<Map>(tmx)["height"] * std::get<Map>(tmx)["tileheight"] + 64; //extra space
  
  //sdl part
  SDL_Window* window = SDL_CreateWindow("pragma", SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED, width, height, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  //object part
  top = new Object(nullptr, -1, -1, -1, "TOP");
  user_choice = new Image(top, 0, 0, 30, "arrow_down", resource_dir.filePath("arrow_down.png"));
  user_choice->hide();
  for (auto it : LayerParser::comming_in_fast().parse(tmx))
    new Image(top, std::get<X>(it), std::get<Y>(it), std::get<Z>(it), "", 
	      resource_dir.filePath(std::get<Source>(it)));
  Image* ego = new Image(top, 100, 380, 20, "Alter_ego", resource_dir.filePath("alter_ego.png"));
  ego->on_mouse_button_up([](Sprite*, const SDL_Event&) {
    top->on_after(&trade);
  });
  Image* a_player = new Image(top, 0, 464, 25, "a_player", resource_dir.filePath("a.png"));
  Image* b_player = new Image(top, 0 + 64, 464, 25, "b_player", resource_dir.filePath("b.png"));
  Image* c_player = new Image(top, 0 + 64 + 64, 464, 25, "c_player", resource_dir.filePath("c.png"));
  Image* me = new Image(top, 0 + 64 + 64 + 64, 464, 25, "me", resource_dir.filePath("me.png"));
  Image* hourglass = new Image(top, 400 - 64, 464, 20, "Hourglass", 
			       resource_dir.filePath("hourglass.png"));
  hourglass->on_mouse_button_up([](Sprite*, const SDL_Event&) {
    pragma.made_move();
  });

  gas = new Text(top, 16 + 2, 15, 20, "GAS", QString::number(pragma.get_player_resource(ME)[GAS]),
		 resource_dir.filePath("Times New Roman Cyr.ttf"), 12, {255, 0, 0, 0});
  Image* gas_icon = new Image(top, 0, 16, 20, "GAS_ICON", resource_dir.filePath("gas.png"));

  oil = new Text(top, 100, 15, 20, "OIL", QString::number(pragma.get_player_resource(ME)[OIL]),
		 resource_dir.filePath("Times New Roman Cyr.ttf"), 12, {255, 0, 0, 0});
  Image* oil_icon = new Image(top, 100 - 16 - 2, 16, 20, "OIL_ICON",
			      resource_dir.filePath("oil.png"));

  metal = new Text(top, 200, 15, 20, "METAL", QString::number(pragma.get_player_resource(ME)[METAL]),
		   resource_dir.filePath("Times New Roman Cyr.ttf"), 12, {255, 0, 0, 0});
  Image* metal_icon = new Image(top, 200 - 16 - 2, 16, 20, "METAL_ICON",
				resource_dir.filePath("metal.png"));

  money = new Text(top, 300, 16, 20, "MONEY", QString::number(pragma.get_player_resource(ME)[MONEY]),
		   resource_dir.filePath("Times New Roman Cyr.ttf"), 12, {255, 0, 0, 0});  
  Image* money_icon = new Image(top, 300 - 16, 16, 20, "MONEY_ICON",
				resource_dir.filePath("money.png"));

  selected = new Image(top, 0, 464, 25, "selected", resource_dir.filePath("select.png"));

  //callback parts
  pragma.on_made_move([]() {
    switch (pragma.get_active_player()) {
      case A:
	selected->set_x(0);
	break;
      case B:
	selected->set_x(64);
	break;
      case C:
	selected->set_x(64 + 64);
	break;
      case ME:
	selected->set_x(64 + 64 + 64);
	break;
    }
  });
  pragma.on_trade([]() {
    gas->set_text(QString::number(pragma.get_player_resource(ME)[GAS])),
    oil->set_text(QString::number(pragma.get_player_resource(ME)[OIL]));
    metal->set_text(QString::number(pragma.get_player_resource(ME)[METAL]));
    money->set_text(QString::number(pragma.get_player_resource(ME)[MONEY]));
  });
  //main cycle part
  while (is_running) {
    Uint32 start = SDL_GetTicks();
    //handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch(event.type) {
	case SDL_QUIT:
	  is_running = false;
	  break;
	default:
	  top->event(event);
	  top->after();
	  break;
      }
    }
     //update
    if (pragma.get_active_player() != ME)
      players[pragma.get_active_player()].make_move(pragma);
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
  
  

