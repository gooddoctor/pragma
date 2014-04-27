#ifndef __OBJECT_H
#define __OBJECT_H

#include <memory>
#include <vector>

#include <QString>

#include <SDL2/SDL.h>
#include <SDL/SDL_ttf.h>

namespace object {
  class Object {
  public:
  typedef std::function<Object*(Object*, const SDL_Event& e)> Handler;
  public:
    Object(Object* parent, int x, int y, int z, const QString& id);
    virtual Object* add(Object* child);
    virtual Object* event(const SDL_Event& e);
    virtual Object* render(SDL_Renderer* sdlrenderer);
    virtual bool contains(int x, int y);
    virtual int h();
    virtual int w();
  private:
    Object* fire_if_contains(int x, int y, const Handler& handler, const SDL_Event& e);
  private:
    std::vector<std::shared_ptr<Object> > children;
  protected:
    virtual Object* mouse_button_up_handler(const SDL_Event& e);
    virtual Object* mouse_motion_handler(const SDL_Event& e);
  protected:
    int x;
    int y;
    int z;
    QString id;
  };
  
  class SurfaceDeleter {
  public:
    void operator()(SDL_Surface* surface);
  };

  class TextureDeleter {
  public:
    void operator()(SDL_Texture* texture);
  };

  SDL_Rect rect(int x, int y, int w, int h);

  extern SDL_Color default_color;
  extern QString default_font;
}

#endif
