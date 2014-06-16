#ifndef __CONFIRMATION_H
#define __CONFIRMATION_H

#include <vector>

#include "image.hpp"

namespace object {
  class Confirmation : public Object {
  public:
    typedef std::function<void()> Callback;
  public:
    Confirmation(Object* parent, int x, int y, int z, const QString& id);
    int h() override;
    int w() override;
    Confirmation* on_approved(const Callback& callback);
    Confirmation* on_denied(const Callback& callback);
  protected:
    void on_fire_handler(Sprite* image, const SDL_Event&);
    void on_select_handler(Sprite* image, const SDL_Event&);
  private:
    Confirmation* fire_callbacks(const std::vector<Callback>& callbacks);
  private:
    Image* approved;
    Image* denied;
    Image* hint;
    std::vector<Callback> approved_callbacks;
    std::vector<Callback> denied_callbacks;
  };
}

#endif
