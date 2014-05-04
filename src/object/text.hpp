#ifndef __TEXT_H
#define __TEXT_H

#include "primitive/sprite.hpp"

namespace object {
  class Text : public Sprite {
  public:
    Text(Object* parent, int x, int y, int z, const QString& id, 
	 const QString& text, const QString& font, int font_size, const SDL_Color& color);
    QString get_text();
    Text* set_text(const QString& text);
  private:
    QString text;
    QString font;
    int font_size;
    SDL_Color color;
  };
}

#endif
