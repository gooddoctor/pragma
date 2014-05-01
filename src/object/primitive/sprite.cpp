#include "sprite.hpp"

using namespace object;

Sprite::Sprite(Object* parent, int x, int y, int z, const QString& id,
	       const SurfaceLoader& loader) : Object(parent, x, y, z, id) {
  surface.reset(loader());
}

Sprite* Sprite::render(SDL_Renderer* sdlrender) {
  if (!texture)
    texture.reset(SDL_CreateTextureFromSurface(sdlrender, surface.get()));
  SDL_Rect src = rect(0, 0, w(), h());
  SDL_Rect dst = rect(x, y - h(), w(), h());
  SDL_RenderCopyEx(sdlrender, texture.get(), &src, &dst, 0, NULL, SDL_FLIP_NONE);
  return this;
}

int Sprite::h() {
  return surface->h;
}

int Sprite::w() {
  return surface->w;
}

Sprite* Sprite::on_mouse_button_up(const Callback& callback) {
  button_up_callbacks.push_back(callback);
  return this;
}

Sprite* Sprite::mouse_button_up_handler(const SDL_Event& e) {
  for (auto it : button_up_callbacks)
    it(this, e);
  return this;
}

