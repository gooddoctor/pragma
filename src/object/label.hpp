#ifndef __LABEL_H
#define __LABEL_H

#include "image.hpp"
#include "text.hpp"

namespace object {
  class Label : public Object {
  public:
    Label(Object* parent, int x, int y, int z, const QString& id, const QString& img,
	  const QString& txt);
    Label* set_text(const QString& txt);
  private:
    Image* background;
    Image* image;
    Text* text;
  };
}

#endif
