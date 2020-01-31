#include "tvxcore.h"

using namespace tvx;

KeyMap keys = KeyMap();
std::unique_ptr<Game> game;
std::shared_ptr<Scene> scene;

void key_press(SDL_Event& e, Scene& s) {
	if(keys.is_action(get_key(e), "Close")) game->stop();
}

int main(int argc, char* argv[]) {
	game = std::make_unique<Game>();
	scene = std::make_shared<Scene>("My Scene");

	keys.add("Close", SDLK_UP);

	game->set_title("My Toyvox Game");
	game->set_resolution(320, 180);

	game->set_current_scene(scene);
	scene->on_key_pressed = key_press;

	game->run();

    return 0;
}