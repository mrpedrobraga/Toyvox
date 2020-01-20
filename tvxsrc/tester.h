#include <stdio.h>
#include "tvxcore.h"

using namespace std;
using namespace tvx;

/* 				SCENE 1 				*/
/*	Alternatively, you can have one
	header for each scene!				*/


//Init Scene and all its handlers
Scene scene1 = Scene(strdup("My Scene"));	
ComponentHandler componentHandler = ComponentHandler();
EntityHandler entityHandler = EntityHandler();

//Init all component sets!
ComponentSet cset_TypeTag = ComponentSet();

/*			  END OF SCENE 1			*/