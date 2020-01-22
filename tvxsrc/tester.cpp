#include <iostream>
#include <stdio.h>
#include "tvxcore.h"
#include "tester.h"

using namespace std;
using namespace tvx;

Game testGame = Game();

void init() {

	scene1.on_load = test_func;

	testGame.set_current_scene(&scene1);

	scene1.add_handler(&entityHandler);
	scene1.add_handler(&componentHandler);

	componentHandler.add("Type Tag", TypeTags);
}

int main(int argc, char* argv[])
{
	cout << "Init:" << endl;
	cout << "hEyy...";

	init();

	cout << testGame.get_current_scene().get_title() << endl;
	EntityUID ent1 = testGame.get_current_scene().create_entity();
	TypeTags.set(ent1, TypeTag("My name"));

	cout << TypeTags.get(ent1).get_value() << endl;

	getchar();
	return 0;
}