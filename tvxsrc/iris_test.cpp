#include "tvxcore.h"
#include "tvxutil.h"

#include <iostream>

tvx::KeyMap keys = tvx::KeyMap();
std::shared_ptr<tvx::Scene> s1;
tvx::EntityHandler s1_entH;
tvx::ComponentHandler s1_compH;
Kinematics s1_Kinematics;

void init()
{
	s1->add_handler(&s1_entH);
	s1->add_handler(&s1_compH);

	s1_compH.add("Kinematics", s1_Kinematics);
}

void key_press(SDL_Event& e, tvx::Scene& s) {
	if(keys.is_action(tvx::game::get_key(e), "Close")) tvx::game::stop();
}

void eventHandler(SDL_Event& e, tvx::Scene& s) {
	if(e.type == SDL_KEYDOWN && keys.is_action(tvx::game::get_key(e), "Switch")) {
			int x, y;
			SDL_GetWindowPosition(tvx::game::window, &x, &y);
			SDL_SetWindowPosition(tvx::game::window, y, x);
	}
}

int main(int argc, char* argv[]) {

	s1 = std::make_shared<tvx::Scene>("Scene 1");
	s1->on_event = eventHandler;
	s1->on_key_pressed = key_press;
	init();

	keys.add("Close", SDLK_UP);
	keys.add("Switch", SDLK_DOWN);

	tvx::game::set_title("My Toyvox Game");
	tvx::game::set_resolution(640, 360);

	tvx::game::set_current_scene(s1);

	tvx::game::init();
	tvx::game::run();

  return 0;
}
