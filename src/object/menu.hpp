#ifndef __MENU_H
#define __MENU_H

#include <vector>

#include "primitive/base/object.hpp"

namespace object {
  class Menu : public Object {
  public:
  typedef std::vector<QString> StringList;
  public:
    Menu(Object* parent, int x, int y, int z, const QString& id, const StringList& entries);
  };
}

#endif
