#include "tvxcore.h"

using namespace tvx;

KeyMap keys = KeyMap();
std::shared_ptr<Scene> scene;

void key_press(SDL_Event& e, Scene& s) {
	if(keys.is_action(game::get_key(e), "Close")) game::stop();
}

int main(int argc, char* argv[]) {

	scene = std::make_shared<Scene>("My Scene");

	keys.add("Close", SDLK_UP);

	game::set_title("My Toyvox Game");
	game::set_resolution(640, 360);

	game::set_current_scene(scene);

	game::launch(SDL_WINDOW_RESIZABLE);

  return 0;
}
