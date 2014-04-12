#include <QDebug>
#include <QDir>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "parser/tmx_parser.hpp"

int main(int, char**) {
  TMXParser::TMX tmx = TMXParser::comming_in_fast().parse("tile/map.tmx");
  int width = std::get<0>(tmx)["width"];
  int height = std::get<0>(tmx)["height"];
  int tilewidth = std::get<0>(tmx)["tilewidth"];
  int tileheight = std::get<0>(tmx)["tileheight"];
  qDebug() << width << height << tilewidth << tileheight;

  SDL_Window* window = SDL_CreateWindow("pragma", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
  						  width * tilewidth, height * tileheight, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  SDL_RenderClear(renderer);

  
  QDir data_dir("tile/source/");
  TMXParser::TMXTileset tileset = std::get<1>(tmx);
  std::vector<SDL_Texture*> textures;
  for (auto it = tileset.begin(); it != tileset.end(); it++) {
    SDL_Surface* surface = IMG_Load(data_dir.filePath(it->second).toStdString().c_str());
    textures.push_back(SDL_CreateTextureFromSurface(renderer, surface));
  }
  
  for (auto it = textures.begin(); it != textures.end(); it++) {
    SDL_RenderCopyEx(renderer, (*it), NULL, NULL, 0, NULL, SDL_FLIP_NONE);
  }

  SDL_Surface* hero = IMG_Load(data_dir.filePath("alter_ego.png").toStdString().c_str());
  SDL_Texture* hero_texture = SDL_CreateTextureFromSurface(renderer, hero);
  SDL_RenderCopyEx(renderer, hero_texture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
  
  SDL_RenderPresent(renderer);
  SDL_Delay(5000);
  SDL_Quit();
}
