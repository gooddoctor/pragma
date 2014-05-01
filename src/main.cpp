#include <QDebug>
#include <QDir>

#include "object/image.hpp"
#include "object/text.hpp"
#include "object/menu.hpp"
#include "parser/parser.hpp"

using namespace object;
using namespace parser;

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

  Object* top = new Object(nullptr, -1, -1, -1, "");
  QDir data_dir("resource/");
  std::vector<TMXObject> objects = LayerParser::comming_in_fast().parse(tmx);
  for (auto it = objects.begin(); it != objects.end(); it++) {
    new Image(top, std::get<X>(*it), std::get<Y>(*it), std::get<Z>(*it), "",
  	       data_dir.filePath(std::get<Source>(*it)));
  }
  Image* img = new Image(top, 100, 380, 10, "Alter_ego", data_dir.filePath("alter_ego.png"));
  img->on_mouse_button_up([](Sprite*, const SDL_Event&) {
			    qDebug() << "okey. its worked";
			  });

  new Text(top, 0, 12, 10, "Sheet", "Привет Мир", data_dir.filePath("Times New Roman Cyr.ttf"),
	   12, {255, 255, 255, 0});

  Menu* menu = new Menu(top, 0, 340, 10, "Menu", {"Кто", "залечит", "рану", "мою"});
  menu->on_select([](Menu*, Text*) {
		    qDebug() << "you wonna live like common people";
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
	break;
      }
    }
  }
}
