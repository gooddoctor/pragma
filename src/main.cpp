#include <QDebug>
#include <QDir>

#include "object/sprite.hpp"
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
  
  Object* top = new Object(nullptr, -1, -1, -1, "");
  QDir data_dir("resource/");
  std::vector<TMXObject> objects = LayerParser::comming_in_fast().parse(tmx);
  for (auto it = objects.begin(); it != objects.end(); it++) {
    new Sprite(top, std::get<X>(*it), std::get<Y>(*it), std::get<Z>(*it), "",
	       data_dir.filePath(std::get<Source>(*it)));
  }
  new Sprite(top, 100, 380, 10, "Alter_ego", data_dir.filePath("alter_ego.png"));

  top->render(renderer);
  SDL_RenderPresent(renderer);
  
  SDL_Event event;
  while (true) {
    SDL_PollEvent(&event);
    switch (event.type) {
      case SDL_QUIT:
	return -1;
    }
  }
}
