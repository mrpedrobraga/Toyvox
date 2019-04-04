
#include <iostream>
#include <stdio.h>
#include "tvxcore.h"

using namespace glm;
using namespace tvx;
using namespace std;

Scene scene;

int main() {

	createTestObject(&scene, vec3(5, 6, 7));

	cout << scene.objectTypeTag[0].tag << endl;

	getchar();
	return 0;
}
