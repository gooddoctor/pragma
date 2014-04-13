#include "object.hpp"

Object::Object(int x, int y, const QString& image) {
  this->x = x;
  this->y = y;
  this->image = image;
}

Object* Object::render(SDL_Renderer* renderer) {
  SDL_Surface* surface = IMG_Load(source.toStdString().c_str());
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect src_rect;
  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.w = surface->w;
  src_rect.h = surface->h;

  SDL_Rect dst_rect;
  dst_rect.x = x;
  dst_rect.y = y;
  dst_rect.w = src_rect.w;
  dst_rect.h = src_rect.h;

  SDL_RenderCopyEx(renderer, texture, &src_rect, &dst_rect, 0, NULL, SDL_FLIP_NONE);
  return this;
}
