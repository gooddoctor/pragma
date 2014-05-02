#include "text.hpp"

#include "menu.hpp"

using namespace object;

Menu::Menu(Object* parent, int x, int y, int z, const QString& id, const StringList& entries) :
      Object(parent, x, y, z, id) {
  set_entries(entries);
}  

Menu* Menu::set_entries(const StringList& entries) {
  int ordinate = y;
  for (auto it : entries) {
    Text* txt = new Text(this, x, ordinate, z, id, it.toStdString().c_str(), 
			 default_font, 12, default_color);
    txt->on_mouse_button_up(std::bind(&Menu::on_select_handler, this, 
			    std::placeholders::_1, std::placeholders::_2));
    ordinate -= txt->h();
  }
  return this;
}

Menu* Menu::on_select(const Callback& callback) {
  select_callbacks.push_back(callback);
  return this;
}

void Menu::on_select_handler(Sprite* text, const SDL_Event&) {
  for (auto it : select_callbacks)
    it(this, dynamic_cast<Text*>(text));
}
