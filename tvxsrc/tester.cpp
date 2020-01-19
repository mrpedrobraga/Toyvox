#include "tvxcore.h"
#include <SDL.h>
#include <stdio.h>

Scene scene;
Object obj1;

int main()
{

	SDL_Init(SDL_INIT_VIDEO);

	scene = Scene("New Layout");
	obj1 = Object("player", vec3(3, 4, 6));
	scene.addObj(&obj1);
	
	printf("%s", scene.getTitle());
	printf("%s", scene.objectAt(0)->getObjectTypeTag());

	getchar();

	SDL_Quit();

	return 0;
}