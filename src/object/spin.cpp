#include "spin.hpp"

using namespace object;

Spin::Spin(Object* parent, int x, int y, int z, const QString& id) : 
      Object(parent, x, y, z, id) {
  up = new Image(this, x, y - 32 - 3, z, id, "object/resource/up.png");
  up->on_mouse_button_up(std::bind(&Spin::inc, this));

  down = new Image(this, x, y, z, id, "object/resource/down.png");
  down->on_mouse_button_up(std::bind(&Spin::dec, this));

  value = new Text(this, x + 50 + 5, y, z, id, "0", "object/resource/default_font.ttf",
 		   62, {255, 255, 255, 0});
}

int Spin::inc() {
  value->set_text(QString::number(value->get_text().toInt() + 1));
  return val();
}

int Spin::dec() {
  value->set_text(QString::number(value->get_text().toInt() - 1));
  return val();
}

int Spin::val() {
  return value->get_text().toInt();
}

int Spin::h() {
  return std::max(up->h() + down->h() + 3, value->h());
}

int Spin::w() {
  return std::max(up->w(), down->w()) + 5 + value->w();
}
