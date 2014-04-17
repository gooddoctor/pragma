#include <tuple>

#include <SDL2/SDL_image.h>

#include "object.hpp"

typedef std::tuple<int, int> Size;

enum {W, H};

class SurfaceDeleter {
public:
  void operator()(SDL_Surface* surface) {
     SDL_FreeSurface(surface);
  }
};

SDL_Rect create_rect(int x, int y, const Size& size) {
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = std::get<W>(size);
  rect.h = std::get<H>(size);
  return rect;
}

Size get_size(SDL_Texture* texture) {
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);
  return std::make_tuple(w, h);
}

std::vector<std::unique_ptr<Object> > Object::all;

Object::Object(int x, int y, const QString& sprite) {
  this->x = x;
  this->y = y;
  this->sprite = sprite;
  all.push_back(std::unique_ptr<Object>(this));
}

Object* Object::render(SDL_Renderer* renderer) {
  if (!texture) {
    std::unique_ptr<SDL_Surface, SurfaceDeleter> surface(IMG_Load(sprite.toStdString().c_str()));
    texture.reset(SDL_CreateTextureFromSurface(renderer, surface.get()));
  }
  SDL_Rect src = create_rect(0, 0, get_size(texture.get()));
  SDL_Rect dst = create_rect(x, y - std::get<H>(get_size(texture.get())), get_size(texture.get()));
  SDL_RenderCopyEx(renderer, texture.get(), &src, &dst, 0, NULL, SDL_FLIP_NONE);
  return this;
}
