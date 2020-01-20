#include <iostream>
#include <stdio.h>
#include <string.h>
#include "tvxcore.h"
#include "tester.h"

using namespace std;
using namespace tvx;


void init() {

	//Register all component sets to the handler using this syntax:
	//componentHandler.add(name, set);

	//Right now it's useless, since there is no "System" functionality... yet...
	componentHandler.add("Type_Tag", TypeTags);
}

int main()
{
	init();
	cout << scene1.get_title() << endl;
	EntityUID ent1 = scene1.create_entity();
	TypeTags.set(ent1, TypeTag("My name"));

	cout << TypeTags.get(ent1).get_value() << endl;

	getchar();
	return 0;
}