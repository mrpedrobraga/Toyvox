
#include "sdlc.h"
#include <stdio.h>

using namespace toyvox;

int main()
{
  SdlContext sdlc("toyvox");
  while (sdlc.pollEvents());
	return 0;
}
