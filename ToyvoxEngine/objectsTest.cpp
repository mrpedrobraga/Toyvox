
#include "tvxcore.h"
#include <stdio.h>

int main()
{
	Scene scene;
	Object obj1;

	scene = Scene("New Layout");
	obj1 = Object("player", vec3(3, 4, 6));
	scene.addObj(&obj1);

	printf("%s", scene.objectAt(0)->getObjectTypeTag());

	getchar();
	return 0;
}
