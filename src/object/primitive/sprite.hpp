#ifndef __SPRITE_H
#define __SPRITE_H

#include "base/object.hpp"

namespace object {
  class Sprite : public Object {
  public:
  typedef std::function<SDL_Surface*(void)> SurfaceLoader;
  public:
    Sprite(Object* parent, int x, int y, int z, const QString& id, const SurfaceLoader& loader);
    Sprite* render(SDL_Renderer* sdlrender) override;
    int h() override;
    int w() override;
  protected:
    std::unique_ptr<SDL_Surface, SurfaceDeleter> surface;
    std::unique_ptr<SDL_Texture, TextureDeleter> texture;
  };
}

#endif
