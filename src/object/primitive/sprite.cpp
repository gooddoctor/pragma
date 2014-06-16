#include "sprite.hpp"

using namespace object;

Sprite::Sprite(Object* parent, int x, int y, int z, const QString& id,
	       const SurfaceLoader& loader) : Object(parent, x, y, z, id) {
  reload(loader);
}

Sprite* Sprite::render(SDL_Renderer* sdlrender) {
  if (!texture)
    texture.reset(SDL_CreateTextureFromSurface(sdlrender, surface.get()));
  SDL_Rect src = rect(0, 0, w(), h());
  SDL_Rect dst = rect(x, y - h(), w(), h());
  SDL_RenderCopyEx(sdlrender, texture.get(), &src, &dst, 0, NULL, SDL_FLIP_NONE);
  if (is_select) {
    SDL_SetRenderDrawColor(sdlrender, 255, 0, 0, 255);
    SDL_RenderDrawRect(sdlrender, &dst);
  }
  return this;
}

Sprite* Sprite::select() {
  is_select = true;
  return this;
}

Sprite* Sprite::deselect() {
  is_select = false;
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

Sprite* Sprite::on_mouse_motion(const Callback& callback) {
  motion_callbacks.push_back(callback);
  return this;
}

Sprite* Sprite::fire_callbacks(const std::vector<Callback>& callbacks, const SDL_Event& e) {
  for (auto it : callbacks)
    it(this, e);
  return this;
}

Sprite* Sprite::reload(const SurfaceLoader& loader) {
  surface.reset(loader());
  texture.reset(nullptr);
  return this;
}

Sprite* Sprite::mouse_button_up_handler(const SDL_Event& e) {
  fire_callbacks(button_up_callbacks, e);
  return this;
}

Sprite* Sprite::mouse_motion_handler(const SDL_Event& e) {
  fire_callbacks(motion_callbacks, e);
  return this;
}
