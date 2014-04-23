#ifndef __OBJECT_H
#define __OBJECT_H

#include <memory>
#include <vector>

#include <QString>

#include <SDL2/SDL.h>

namespace object {
  class Object {
  public:
    Object(Object* parent, int x, int y, int z, const QString& id);
    Object* add(Object* child);
    virtual Object* render(SDL_Renderer* sdlrenderer);
  private:
    std::vector<std::shared_ptr<Object> > children;
  protected:
    int x;
    int y;
    int z;
    QString id;
  };
}

#endif
