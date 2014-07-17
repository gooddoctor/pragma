#ifndef __TEXT_H
#define __TEXT_H

#include <SDL2/SDL_ttf.h>

#include "primitive/sprite.hpp"

namespace object {
  class Text : public Sprite {
  public:
    Text(Object* parent, int x, int y, int z, const QString& id, 
	 const QString& text, const QString& font = default_font, int font_size = 12, 
	 const SDL_Color& color = default_color);
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
