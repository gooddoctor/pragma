#include "text.hpp"

using namespace object;

SDL_Surface* load_text(const QString& text, const QString& font, 
		       int font_size, SDL_Color color) {
  return TTF_RenderUTF8_Solid(TTF_OpenFont(font.toStdString().c_str(), font_size),
			      text.toStdString().c_str(), color);
}

Text::Text(Object* parent, int x, int y, int z, const QString& id, 
           const QString& text, const QString& font, int font_size, const SDL_Color& color) :
      Sprite(parent, x, y, z, id, std::bind(load_text, text, font, font_size, color)) {
  this->text = text;
  this->font = font;
  this->font_size = font_size;
  this->color = color;
}

QString Text::get_text() {
  return text;
}

Text* Text::set_text(const QString& text) {
  this->text = text;
  reload(std::bind(load_text, this->text, this->font, this->font_size, this->color));
  return this;
}
