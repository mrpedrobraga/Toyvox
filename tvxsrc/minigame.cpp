#define SDL_MAIN_HANDLED

#include <iostream>
#include <stdio.h>

#include "tvxcore.h"

int main(int argc, char* argv[]) {

	tvx::Game testGame = tvx::Game();
	testGame.set_title("My Toyvox Game");
	testGame.set_resolution(320, 180);    

	testGame.run();

    return 0;
}