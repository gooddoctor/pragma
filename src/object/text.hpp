#ifndef __TEXT_H
#define __TEXT_H

#include "primitive/sprite.hpp"

namespace object {
  class Text : public Sprite {
  public:
  Text(Object* parent, int x, int y, int z, const QString& id, 
       const QString& text, const QString& font, int font_size, const SDL_Color& color);
  };
}

#endif
