#include "tvxcore.h"

using namespace tvx;

KeyMap keys = KeyMap();
std::shared_ptr<Scene> scene;

void key_press(SDL_Event& e, Scene& s) {
	if(keys.is_action(game::get_key(e), "Close")) game::stop();
}

void eventHandler(SDL_Event& e, Scene& s) {
	if(e.type == SDL_KEYDOWN && keys.is_action(game::get_key(e), "Switch")) {
			int x, y;
			SDL_GetWindowPosition(game::window, &x, &y);
			SDL_SetWindowPosition(game::window, y, x);
	}
}

int main(int argc, char* argv[]) {

	scene = std::make_shared<Scene>("My Scene");

	scene->on_event = eventHandler;
	//scene->on_key_pressed = key_press;

	keys.add("Close", SDLK_UP);
	keys.add("Switch", SDLK_DOWN);

	game::set_title("My Toyvox Game");
	game::set_resolution(640, 360);

	game::set_current_scene(scene);

	game::launch(SDL_WINDOW_RESIZABLE);

  return 0;
}
