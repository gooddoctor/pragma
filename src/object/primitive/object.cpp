#include "object.hpp"

using namespace object;

Object::Object(Object* parent, int x, int y, int z, const QString& id) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->id = id;
  if (parent)
    parent->add(this);
}

Object* Object::add(Object* child) {
  children.push_back(std::shared_ptr<Object>(child));
  return this;
}

Object* Object::render(SDL_Renderer* sdlrenderer) {
  for (auto it : children)
    it->render(sdlrenderer);
  return this;
}
