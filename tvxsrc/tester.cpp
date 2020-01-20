#include <iostream>
#include <stdio.h>
#include "tvxcore.h"
#include "tester.h"

using namespace std;
using namespace tvx;


void init() {

	//Register all component sets to the handler using this syntax:
	//componentHandler.add(name, set);
	componentHandler.add("Type_Tag", cset_TypeTag);
}

int main()
{

	init();

	cout << scene1.get_title() << endl;

	EntityUID ent1 = scene1.create_entity();
	EntityUID ent2 = scene1.create_entity();
	scene1.destroy_entity(&ent2);
	EntityUID ent3 = scene1.create_entity();

	cout << "Entity 1: " << ent1 << endl << "Entity 2: " << ent2 << endl << "Entity 3: " << ent3 << endl;

	getchar();
	return 0;
}