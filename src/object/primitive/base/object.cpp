#include "object.hpp"

using namespace object;

std::queue<Object::Callback> Object::after_callbacks;

Object::Object(Object* parent, int x, int y, int z, const QString& id) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->id = id;
  if (parent)
    parent->add(this);
}

Object::~Object() { }

Object* Object::add(Object* child) {
  children.push_back(std::shared_ptr<Object>(child));
  return this;
}

Object* Object::after() {
  while (!after_callbacks.empty()) {
    after_callbacks.front()();
    after_callbacks.pop();
  }
  return this;
}

Object* Object::event(const SDL_Event& e) {
  switch (e.type) {
  case SDL_MOUSEBUTTONUP:
    mouse_button_up_handler(e);
    break;
  case SDL_MOUSEMOTION:
    mouse_motion_handler(e);
    break;
  }
  return this;
}

Object* Object::remove_children() {
  children.erase(children.begin(), children.end());
  return this;
}

Object* Object::render(SDL_Renderer* sdlrenderer) {
  for (auto it : children)
    it->render(sdlrenderer);
  return this;
}

bool Object::contains(int x, int y) {
  return (x >= this->x) && (x <= this->x + w()) &&
	 (y <= this->y) && (y >= this->y - h());
}

int Object::h() {
  int bottom = y;
  for (auto it : children)
    bottom -= it->h();
  return y - bottom;
}

int Object::w() {
  int width = 0;
  for (auto it : children)
    if (width < it->w())
      width = it->w();
  return width;
}

Object* Object::on_after(const Callback& callback) {
  after_callbacks.push(callback);
  return this;
}

Object* Object::fire_if_contains(int x, int y, const Handler& handler, const SDL_Event& e) {
  for (auto it : children)
    if (it->contains(x, y))
      handler(it.get(), e);
  return this;
}

Object* Object::mouse_button_up_handler(const SDL_Event& e) {
  return fire_if_contains(e.button.x, e.button.y, &Object::mouse_button_up_handler, e);
}

Object* Object::mouse_motion_handler(const SDL_Event& e) {
  return fire_if_contains(e.motion.x, e.motion.y, &Object::mouse_motion_handler, e);
}


void SurfaceDeleter::operator()(SDL_Surface* surface) {
  SDL_FreeSurface(surface);
}

void TextureDeleter::operator()(SDL_Texture* texture) {
  SDL_DestroyTexture(texture);
}

SDL_Rect object::rect(int x, int y, int w, int h) {
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;
}

SDL_Color object::default_color = {255, 255, 255, 255};

QString object::default_font = "resource/Times New Roman Cyr.ttf";
