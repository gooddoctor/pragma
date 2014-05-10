#include <QDebug>
#include <QDir>

#include "object/image.hpp"
#include "object/text.hpp"
#include "object/menu.hpp"
#include "object/spin.hpp"
#include "parser/parser.hpp"

using namespace object;
using namespace parser;

QDir data_dir("resource/");

void create_menu(Object* top);
void spin_box(Object* top);

void create_menu(Object* top) {
  Menu* menu = new Menu(top, 0, 400, 20, "Menu", {"ОТМЕНА ОПЕРАЦИИ", "Купить"});
  menu->on_select([top](Menu* m, Text* t) {
    if (t->get_text() == "Купить") {
      top->on_after(std::bind(&Object::remove, top, m));
      top->on_after(std::bind(spin_box, top));
    } else {
      top->on_after(std::bind(&Object::remove, top, m));
    }
  });
}
      
void spin_box(Object* top) {
  Spin* spin = new Spin(top, 32, 400, 20, "Spin");
  Image* up = new Image(top, 0, 400 - 32, 20, "up", data_dir.filePath("thumb_up.png"));
  Image* down = new Image(top, 0, 400, 20, "down", data_dir.filePath("thumb_down.png"));
  
  up->on_mouse_button_up([top, spin, up, down](Sprite*, const SDL_Event&) {
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, up));
    top->on_after(std::bind(&Object::remove, top, down));
  });

  down->on_mouse_button_up([top, spin, up, down](Sprite*, const SDL_Event&) {
    top->on_after(std::bind(&Object::remove, top, spin));
    top->on_after(std::bind(&Object::remove, top, up));
    top->on_after(std::bind(&Object::remove, top, down));
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

  std::vector<TMXObject> objects = LayerParser::comming_in_fast().parse(tmx);
  for (auto it = objects.begin(); it != objects.end(); it++) {
    new Image(top, std::get<X>(*it), std::get<Y>(*it), std::get<Z>(*it), "",
  	       data_dir.filePath(std::get<Source>(*it)));
  }

  Image* img = new Image(top, 100, 380, 20, "Alter_ego", data_dir.filePath("alter_ego.png"));
  img->on_mouse_button_up([top](Sprite*, const SDL_Event&) {
    top->on_after(std::bind(create_menu, top));
  });

  top->render(renderer);
  SDL_RenderPresent(renderer);
  
  SDL_Event event;
  while (true) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
	return -1;
	break;
      default:
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
