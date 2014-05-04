#ifndef __SPIN_H
#define __SPIN_H

#include "image.hpp"
#include "text.hpp"

namespace object {
  class Spin : public Object {
  public:
    Spin(Object* parent, int x, int y, int z, const QString& id);
    int inc();
    int dec();
    int val();
    int h() override;
    int w() override;
  private:
    Image* up;
    Image* down;
    Text* value;
  };
}

#endif
