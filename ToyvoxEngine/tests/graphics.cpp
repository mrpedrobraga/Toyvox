
#include <stdio.h>
#include "tvxcore.h"

using namespace tvx;

int main()
{
  SdlContext sdlc("toyvox");
  while (sdlc.pollEvents()) {
    sdlc.clearColor();
    sdlc.swapWindow();
  }
	return 0;
}
