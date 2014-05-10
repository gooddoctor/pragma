#ifndef __OBJECT_H
#define __OBJECT_H

#include <memory>
#include <queue>
#include <vector>

#include <QString>

#include <SDL2/SDL.h>
#include <SDL/SDL_ttf.h>

namespace object {
  class Object {
  public:
  typedef std::function<void(void)> Callback;
  typedef std::function<Object*(Object*, const SDL_Event& e)> Handler;
  public:
    Object(Object* parent, int x, int y, int z, const QString& id);
    virtual ~Object() = default;
    virtual int get_x();
    virtual Object* set_x(int x);
    virtual int get_y();
    virtual Object* set_y(int y);
    virtual int get_z();
    virtual QString get_id();
    virtual Object* add(Object* child);
    virtual Object* after();
    virtual Object* event(const SDL_Event& e);
    virtual Object* remove(Object* child);
    virtual Object* remove_all();
    virtual Object* render(SDL_Renderer* sdlrenderer);
    virtual Object* show();
    virtual bool is_show();
    virtual Object* hide();
    virtual bool contains(int x, int y);
    virtual int h();
    virtual int w();
    virtual Object* on_after(const Callback& callback);
    virtual QString to_string();
  private:
    Object* fire_if_contains(int x, int y, const Handler& handler, const SDL_Event& e);
  private:
    static std::queue<Callback> after_callbacks;
    bool is_hide = false;
  private:
    std::vector<std::shared_ptr<Object> > children;
  protected:
    virtual Object* mouse_button_up_handler(const SDL_Event& e);
    virtual Object* mouse_motion_handler(const SDL_Event& e);
    virtual QString to_string(int tab);
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
