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

int Object::get_x() {
  return x;
}

int Object::get_y() {
  return y;
}

int Object::get_z() {
  return z;
}

QString Object::get_id() {
  return id;
}

Object* Object::add(Object* child) {
  children.push_back(std::shared_ptr<Object>(child));
  std::sort(children.begin(), children.end(),
	    [](std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
	      return first->get_z() < second->get_z();
	    });
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

Object* Object::remove(Object* child) {
  children.erase(std::find_if(children.begin(), children.end(),
			      [child](std::shared_ptr<Object> ptr) {
				return ptr.get() == child;
			      }));
  return this;
}

Object* Object::remove_all() {
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

QString Object::to_string() {
  return to_string(0);
}

Object* Object::fire_if_contains(int x, int y, const Handler& handler, const SDL_Event& e) {
  for (auto it = children.rbegin(); it != children.rend(); it++) {
    if ((*it)->contains(x, y)) {
      handler(it->get(), e);
      break;
    }
  }
  return this;
}

Object* Object::mouse_button_up_handler(const SDL_Event& e) {
  return fire_if_contains(e.button.x, e.button.y, &Object::mouse_button_up_handler, e);
}

Object* Object::mouse_motion_handler(const SDL_Event& e) {
  return fire_if_contains(e.motion.x, e.motion.y, &Object::mouse_motion_handler, e);
}

QString Object::to_string(int tab) {
  QString str = QString("%1%2:%3\n").arg("", tab).arg(id).arg(z);
  for (auto it : children)
    str.append(it->to_string(tab + 2));
  return str;
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
