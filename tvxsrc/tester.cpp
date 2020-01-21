#include <iostream>
#include <stdio.h>
#include <string.h>
#include <glm/glm.hpp>
#include "tvxcore.h"
#include "tester.h"

using namespace std;
using namespace tvx;
using namespace glm;

Game testGame = Game();

void init()
{
	//Register all handlers to scene1
	scene1.add_handler( &componentHandler);
	scene1.add_handler(  &entityHandler  );
	//Register all component sets to the handler using this syntax:
	//componentHandler.add(name, set);

	componentHandler.add("Type_Tag", TypeTags);

	//Register the scene's events!
	scene1.on_load = &testFunc;

	//INITIALIZE OUR GAME!
	testGame.set_title("Game Title!");
	testGame.set_current_scene(&scene1);
	testGame.set_resolution(ivec2(640, 360));
}

int main()
{
	init();
	cout << testGame.get_current_scene().get_title() << endl;
	EntityUID ent1 = testGame.get_current_scene().create_entity();
	TypeTags.set(ent1, TypeTag("My name"));

	cout << TypeTags.get(ent1).get_value() << endl;

	getchar();
	return 0;
}