#ifndef __OBJECT_H
#define __OBJECT_H

#include <memory>
#include <vector>

#include <QString>

#include <SDL2/SDL.h>

class Object {
public:
  Object(int x, int y, const QString& sprite);
  Object* render(SDL_Renderer* sdlrender);
public:
  static std::vector<std::unique_ptr<Object> > all;
private:
  class TextureDeleter {
  public:
    void operator()(SDL_Texture* texture) {
      SDL_DestroyTexture(texture);
    }
  };
private:
  int x;
  int y;
  QString sprite;
  std::unique_ptr<SDL_Texture, TextureDeleter> texture;
};

#endif
