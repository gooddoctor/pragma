#ifndef __SPRITE_H
#define __SPRITE_H

#include "primitive/object.hpp"

namespace object {
  class Sprite : public Object {
  public:
    Sprite(Object* parent, int x, int y, int z, const QString& id, const QString& source);
    Sprite* render(SDL_Renderer* sdlrender) override;
  private:
    class TextureDeleter {
    public:
      void operator()(SDL_Texture* texture) {
	SDL_DestroyTexture(texture);
      }
    };
  private:
    QString source;
    std::unique_ptr<SDL_Texture, TextureDeleter> texture;
  };
}

#endif
