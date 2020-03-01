#include "tvxcore.h"
#include "tvxutil.h"

#include "voxels.h"
#include <iostream>

int main(int argc, char** argv)
{
	tvx::game::init();
	tvx::game::run();

	std::pair<size_t, size_t> palette_sizes = get_CN_palette_sizes(7);

	Palette<Colour, 192> c;
	Palette<Normal, 168> n;

	std::cout << palette_sizes.first << "; " << palette_sizes.second;

	return 0;
}
