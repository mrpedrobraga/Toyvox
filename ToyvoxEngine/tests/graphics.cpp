
#include <stdio.h>
#include "tvxcore.h"

using namespace tvx;

int main() {
  float colorCycler = 0; // used in the color cycling

  SdlContext sdlc("Toyvox Engine");
  while (sdlc.pollEvents()) {

    // blank out the frame with a solid color
    sdlc.clearColor();
    // get time elapsed to do game logic with
    float dt = sdlc.getDeltaTime();

    // silly demo of using dt to cycle background color
    colorCycler += .5f * dt - (float)(colorCycler > 1.f);
    sdlc.setClearColor(
          fabs(sinf(colorCycler * M_PI)),                 // r
          fabs(sinf(colorCycler * M_PI + M_PI * 0.33)),   // g
          fabs(sinf(colorCycler * M_PI + M_PI * 0.66)));  // b

    // TODO: draw things here

    // present the drawn stuff to the display
    sdlc.swapWindow();

  }
	return 0;
}
