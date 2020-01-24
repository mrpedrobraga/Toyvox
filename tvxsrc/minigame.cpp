#include <iostream>
#include <stdio.h>

#include "tvxcore.h"

tvx::Game myGame = tvx::Game();
tvx::Scene scene = tvx::Scene("My Scene");

void key_press(SDL_Event& e, tvx::Scene& s) {

	if(tvx::get_key(e) == SDLK_UP) myGame.stop();
}

int main(int argc, char* argv[]) {

	myGame.set_title("My Toyvox Game");
	myGame.set_resolution(320, 180);    

	myGame.set_current_scene(&scene);
	scene.on_key_pressed = key_press;

	myGame.run();

    return 0;
}