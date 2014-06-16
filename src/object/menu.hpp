#ifndef __MENU_H
#define __MENU_H

#include <vector>

#include "image.hpp"
#include "text.hpp"

namespace object {
  class Menu : public Object {
  public:
  typedef std::function<void(Menu*, Text*)> Callback;
  typedef std::vector<QString> StringList;
  public:
    Menu(Object* parent, int x, int y, int z, const QString& id, const StringList& entries);
    Menu* set_entries(const StringList& entries);
    Menu* on_select(const Callback& callback);
  private:
    Image* hint; 
    std::vector<Callback> select_callbacks;
  protected:
    void on_select_handler(Sprite* text, const SDL_Event&);
    void on_fire_handler(Sprite* text, const SDL_Event&);
  };
}

#endif
