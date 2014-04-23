#include <SDL2/SDL_image.h>

#include "sprite.hpp"

using namespace object;

//********************************************************************************
typedef std::tuple<int, int> Size;
enum {W, H};
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
//********************************************************************************

class SurfaceDeleter {
public:
  void operator()(SDL_Surface* surface) {
    SDL_FreeSurface(surface);
  }
};

Sprite::Sprite(Object* parent, int x, int y, int z, const QString& id, const QString& source) : 
	Object(parent, x, y, z, id) {
  this->source = source;
}

Sprite* Sprite::render(SDL_Renderer* sdlrender) {
  Object::render(sdlrender);
  if (!texture) {
    std::unique_ptr<SDL_Surface, SurfaceDeleter> surface(IMG_Load(source.toStdString().c_str()));
    texture.reset(SDL_CreateTextureFromSurface(sdlrender, surface.get()));
  }
  SDL_Rect src = create_rect(0, 0, get_size(texture.get()));
  SDL_Rect dst = create_rect(x, y - std::get<H>(get_size(texture.get())), get_size(texture.get()));
  SDL_RenderCopyEx(sdlrender, texture.get(), &src, &dst, 0, NULL, SDL_FLIP_NONE);
  return this;
}
