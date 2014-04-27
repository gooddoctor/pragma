#include "text.hpp"

#include "menu.hpp"

using namespace object;

Menu::Menu(Object* parent, int x, int y, int z, const QString& id, const StringList& entries) :
      Object(parent, x, y, z, id) {
  int ordinate = y;
  for (auto it : entries) {
    Text* txt = new Text(this, x, ordinate, z, id, it.toStdString().c_str(), 
			 default_font, 12, default_color);
    ordinate -= txt->h();
  }
}  
