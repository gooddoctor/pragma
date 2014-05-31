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

Game pragma;

QDir data_dir("resource/");

Image* arrow;

void count(Object* top, QString text, std::function<void(RESOURCE, int)> action) {
  Spin* spin = new Spin(top, 0, 400, 20, "count");
  Image* approved = new Image(top, 0, 400 - 60, 20, "approved", data_dir.filePath("approved.png"));
  Image* denied = new Image(top, 64 + 2, 400 - 60, 20, "denied", data_dir.filePath("denied.png"));
  approved->on_mouse_button_up([top, spin, approved, denied, text, action](Sprite*, const SDL_Event&) {
     top->on_after(std::bind(&Object::remove, top, spin));
     top->on_after(std::bind(&Object::remove, top, approved));
     top->on_after(std::bind(&Object::remove, top, denied));
     if (text == "GAS")
       action(game::RESOURCE::GAS, spin->val());
     else if (text == "OIL")
       action(game::RESOURCE::OIL, spin->val());
     else if (text == "METAL")
       action(game::RESOURCE::METAL, spin->val());
     arrow->hide();
     qDebug() << pragma.to_string();
  });
  approved->on_mouse_motion([](Sprite* s, const SDL_Event&) {
    arrow->show();
    arrow->set_x(s->get_x() + (s->w() - arrow->w()) / 2);
    arrow->set_y(s->get_y() - s->h());
  });
  denied->on_mouse_button_up([top, spin, approved, denied, text](Sprite*, const SDL_Event&) {
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, approved));
    top->on_after(std::bind(&Object::remove, top, denied));
    arrow->hide();
  });
  denied->on_mouse_motion([](Sprite* s, const SDL_Event&) {
    arrow->show();
    arrow->set_x(s->get_x() + (s->w() - arrow->w()) / 2);
    arrow->set_y(s->get_y() - s->h());
  });
}

void buy(Object* top) {
  Menu* menu = new Menu(top, 0, 400, 20, "Buy", {"ОТМЕНА ОПЕРАЦИИ", "GAS", "OIL", "METAL"});
  menu->on_select([top](Menu* m, Text* t) {
    if (t->get_text() != "ОТМЕНА ОПЕРАЦИИ") {
      top->on_after(std::bind(&Object::remove, top, m));
      top->on_after(std::bind(count, top, t->get_text(), [](RESOURCE r, int a) {
							   pragma.player_bought(r, a);
							 }));
    } else {
      top->on_after(std::bind(&Object::remove, top, m));
    }
  });
}

void sell(Object* top) {
  Menu* menu = new Menu(top, 0, 400, 20, "Sell", {"ОТМЕНА ОПЕРАЦИИ", "GAS", "OIL", "METAL"});
  menu->on_select([top](Menu* m, Text* t) {
    if (t->get_text() != "ОТМЕНА ОПЕРАЦИИ") {
      top->on_after(std::bind(&Object::remove, top, m));
      top->on_after(std::bind(count, top, t->get_text(), [](RESOURCE r, int a) {
							   pragma.player_sold(r, a);
							 }));
    } else {
      top->on_after(std::bind(&Object::remove, top, m));
    }
  });
}


void trade(Object* top) {
  Menu* menu = new Menu(top, 0, 400, 20, "Trade", {"ОТМЕНА ОПЕРАЦИИ", "Продать", "Купить"});
  menu->on_select([top](Menu* m, Text* t) {
    if (t->get_text() == "Купить") {
      top->on_after(std::bind(&Object::remove, top, m));
      top->on_after(std::bind(buy, top));
    } else if (t->get_text() == "Продать") {
      top->on_after(std::bind(&Object::remove, top, m));
      top->on_after(std::bind(sell, top));
    } else {
      top->on_after(std::bind(&Object::remove, top, m));
    }
  });
}


int main(int , char**) {
  TMX tmx = TMXParser::comming_in_fast().parse("resource/map.tmx");
  int width = std::get<Map>(tmx)["width"] * std::get<Map>(tmx)["tilewidth"];
  int height = std::get<Map>(tmx)["height"] * std::get<Map>(tmx)["tileheight"];
  
  SDL_Window* window = SDL_CreateWindow("pragma", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
					width, height, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  SDL_RenderClear(renderer);
  
  TTF_Init();

  Object* top = new Object(nullptr, -1, -1, -1, "TOP");

  arrow = new Image(top, 0, 0, 30, "arrow_down", data_dir.filePath("arrow_down.png"));
  arrow->hide();

  std::vector<TMXObject> objects = LayerParser::comming_in_fast().parse(tmx);
  for (auto it = objects.begin(); it != objects.end(); it++) {
    new Image(top, std::get<X>(*it), std::get<Y>(*it), std::get<Z>(*it), "",
  	       data_dir.filePath(std::get<Source>(*it)));
  }

  Image* img = new Image(top, 100, 380, 20, "Alter_ego", data_dir.filePath("alter_ego.png"));
  img->on_mouse_button_up([top](Sprite*, const SDL_Event&) {
    top->on_after(std::bind(trade, top));
  });

  Text* active_player = new Text(top, 0, 24, 20, "ACTIVE_PLAYER",
				 "Active player: " + game::PLAYER_to_str[pragma.get_active_player()],
				 data_dir.filePath("Times New Roman Cyr.ttf"),
				 24, {120, 120, 120, 0});

  Image* hourglass = new Image(top, 400 - 64, 400, 20, "Hourglass", 
			       data_dir.filePath("hourglass.png"));
  hourglass->on_mouse_button_up([active_player](Sprite*, const SDL_Event&) {
    pragma.player_made_move();
  });

  pragma.on_player_made_move([active_player]() {
    active_player->set_text("Active player:" + game::PLAYER_to_str[pragma.get_active_player()]);
  });

  top->render(renderer);
  SDL_RenderPresent(renderer);
  
  std::map<PLAYER, Player> players{{A, Player()}, {B, Player()}, {C, Player()}};

  SDL_Event event;
  while (true) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
	return -1;
	break;
      default:
	if (pragma.get_active_player() != ME)
	  players[pragma.get_active_player()].make_move(pragma);
	top->event(event);
	top->after();
	SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);
        top->render(renderer);
        SDL_RenderPresent(renderer);
	break;
      }
    }
  }
}
