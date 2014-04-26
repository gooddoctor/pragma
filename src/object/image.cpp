#include <SDL2/SDL_image.h>

#include "image.hpp"

using namespace object;

SDL_Surface* load_image(const QString& image) {
  return IMG_Load(image.toStdString().c_str());
}

Image::Image(Object* parent, int x, int y, int z, const QString& id, const QString& image) : 
	     Sprite(parent, x, y, z, id, std::bind(load_image, image)) { /* */ }
