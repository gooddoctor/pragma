#ifndef __IMAGE_H
#define __IMAGE_H

#include "primitive/sprite.hpp"

namespace object {
  class Image : public Sprite {
  public:
    Image(Object* parent, int x, int y, int z, const QString& id, const QString& image);
  };
}

#endif
