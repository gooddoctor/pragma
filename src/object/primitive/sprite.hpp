#ifndef __SPRITE_H
#define __SPRITE_H

#include "base/object.hpp"

namespace object {
  class Sprite : public Object {
  public:
  typedef std::function<SDL_Surface*(void)> SurfaceLoader;
  typedef std::function<void(Sprite*, const SDL_Event&)> Callback;
  public:
    Sprite(Object* parent, int x, int y, int z, const QString& id, const SurfaceLoader& loader);
    Sprite* render(SDL_Renderer* sdlrender) override;
    int h() override;
    int w() override;
    Sprite* on_mouse_button_up(const Callback& callback);
  protected:
    Sprite* reload(const SurfaceLoader& loader);
    Sprite* mouse_button_up_handler(const SDL_Event& e) override;
  protected:
    std::vector<Callback> button_up_callbacks;
    std::unique_ptr<SDL_Surface, SurfaceDeleter> surface;
    std::unique_ptr<SDL_Texture, TextureDeleter> texture;
  };
}

#endif
