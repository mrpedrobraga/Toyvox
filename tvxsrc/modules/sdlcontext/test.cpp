
#include <cmath>
#include "sdlc.hpp"

using namespace tvx;

int main(int argc, char **argv) {
	float colorCycler = 0; // used in the color cycling

	SdlContext sdlc("Toyvox SDL Context Test");

	// can set up stuff that uses opengl here

	while (sdlc.pollEvents()) {

		// blank out the frame with a solid color
		sdlc.clearColor();
		// get time elapsed to do game logic with
		float dt = sdlc.getDeltaTime();

		// placeholder demo of using dt to cycle background color
		colorCycler += .5f * dt - (float)(colorCycler > 1.f);
		sdlc.setClearColor(
					std::fabs(sinf(colorCycler * (float)M_PI)),                         // r
					std::fabs(sinf(colorCycler * (float)M_PI + (float)M_PI * 0.33f)),   // g
					std::fabs(sinf(colorCycler * (float)M_PI + (float)M_PI * 0.66f)));  // b

		// can draw the stuff here

		// present the drawn stuff to the display
		sdlc.swapWindow();
	}
	return 0;
}