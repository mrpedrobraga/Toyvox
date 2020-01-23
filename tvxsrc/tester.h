#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include "tvxcore.h"

using namespace std;
using namespace tvx;

/* 				SCENE 1 				*/
/*	Alternatively, you can have one
	header for each scene!				*/

//Init Scene and all its handlers
Scene scene1 = Scene("My Scene");	
ComponentHandler componentHandler = ComponentHandler();
EntityHandler entityHandler = EntityHandler();

//Init all component sets!
ComponentSet<glm::vec2> Positions2D = ComponentSet<glm::vec2>();

/*			  END OF SCENE 1			*/

/* END OF SYSTEMS*/