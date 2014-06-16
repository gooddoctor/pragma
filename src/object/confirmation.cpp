#include "confirmation.hpp"

using namespace object;

Confirmation::Confirmation(Object* parent, int x, int y, int z, const QString& id) :
	      Object(parent, x, y, z, id) {
  approved = new Image(this, x, y, z, id, "object/resource/approved.png");
  approved->on_mouse_motion(std::bind(&Confirmation::on_fire_handler, this,
				      std::placeholders::_1, std::placeholders::_2));
  approved->on_mouse_button_up(std::bind(&Confirmation::on_select_handler, this, 
					 std::placeholders::_1, std::placeholders::_2));

  denied = new Image(this, x + approved->w() + 2, y, z, id, "object/resource/denied.png");
  denied->on_mouse_motion(std::bind(&Confirmation::on_fire_handler, this,
				    std::placeholders::_1, std::placeholders::_2));
  denied->on_mouse_button_up(std::bind(&Confirmation::on_select_handler, this, 
				       std::placeholders::_1, std::placeholders::_2));

  hint = new Image(this, x, y, z, id, "object/resource/confirmation_hint.png");
  hint->hide();
}

int Confirmation::h() {
  return std::max(approved->h(), denied->h());
}

int Confirmation::w() {
  return approved->w() + denied->w() + 2;
}

Confirmation* Confirmation::on_approved(const Callback& callback) {
  approved_callbacks.push_back(callback);
  return this;
}

Confirmation* Confirmation::on_denied(const Callback& callback) {
  denied_callbacks.push_back(callback);
  return this;
}

void Confirmation::on_fire_handler(Sprite* image, const SDL_Event&) {
  hint->show();
  hint->set_x(image->get_x() + (image->w() - hint->w()) / 2);
  hint->set_y(image->get_y() - image->h());
}

void Confirmation::on_select_handler(Sprite* image, const SDL_Event&) {
  if (image == approved) 
    fire_callbacks(approved_callbacks);
  else if (image == denied)
    fire_callbacks(denied_callbacks);
}

Confirmation* Confirmation::fire_callbacks(const std::vector<Callback>& callbacks) {
  for (auto it : callbacks) it();
  return this;
}  
