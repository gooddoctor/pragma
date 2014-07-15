#include "label.hpp"

using namespace object;

Label::Label(Object* parent, int x, int y, int z, const QString& id, const QString& img,
	     const QString& txt) : Object(parent, x, y, z, id) {
  background = new Image(this, x, y, z - 1, id, "object/resource/label_background.png");
  if (img.isEmpty()) {
    image = nullptr;
    text = new Text(this, x + 5, y, z, id, txt, default_font, 12, default_color);
  } else {
    image = new Image(this, x + 5, y, z, id, img);
    text = new Text(this, x + image->w() + 2 + 5, y, z, id, txt, default_font, 12, default_color);
  }
}

Label* Label::set_text(const QString& txt) {
  text->set_text(txt);
  return this;
}
