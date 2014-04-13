#ifndef __OBJECT_H
#define __OBJECT_H

#include <QString>

#include <SDL2/SDL.h>

class Object {
public:
  Object(int x, int y, const QString& image);
  Object* render(SDL_Renderer* renderer);
private:
  int x;
  int y;
  QString image;
};

#endif
