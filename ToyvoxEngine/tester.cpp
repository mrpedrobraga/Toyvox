#pragma once
#include "tvxcore.h"
#include <stdio.h>

Scene scene;
Object obj1;

int main()
{
	scene = Scene("New Layout");
	obj1 = Object("player", vec3(3, 4, 6));
	scene.addObj(&obj1);
	
	printf("%s", scene.objectAt(0)->getObjectTypeTag());

	getchar();
	return 0;
}