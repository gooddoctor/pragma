#include "object.hpp"

Object::Object(int x, int y, const QString& image) {
  this->x = x;
  this->y = y;
  this->image = image;
}

Object* Object::render(SDL_Renderer* renderer) {
  return this;
}
