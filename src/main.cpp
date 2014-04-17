#include <QDebug>
#include <QDir>

#include <SDL2/SDL.h>

#include "parser/parser.hpp"
#include "object/object.hpp"

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
  
  QDir data_dir("resource/");
  std::vector<TMXObject> objects = LayerParser::comming_in_fast().parse(tmx);
  for (auto it = objects.begin(); it != objects.end(); it++) {
    new Object(std::get<X>(*it), std::get<Y>(*it), data_dir.filePath(std::get<Sprite>(*it)));
  }
  new Object(100, 380, data_dir.filePath("alter_ego.png"));

  for (auto it = Object::all.begin(); it != Object::all.end(); it++) {
    (*it)->render(renderer);
  }
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
