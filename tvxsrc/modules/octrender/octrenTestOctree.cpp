
#include <SDL_log.h>
#include "octren.hpp"

void showNearestNeighbour(Octree<unsigned> &octree, unsigned x, unsigned y, unsigned z) {
	SDL_Log("Search for nearest element to position (%u, %u, %u)\n", x, y, z);
	unsigned found_x;
	unsigned found_y;
	unsigned found_z;
	auto leaf = octree.findNearestNeighbour(x, y, z, found_x, found_y, found_z);
	if (leaf) {
		SDL_Log("\t=> Found nearest element at (%u, %u, %u) with value %u\n", found_x, found_y, found_z, leaf->value());
	} else {
		SDL_Log("\t=> No nearest element found - octree empty?\n");
	}
}

int main(int argc, char **argv) {	
	SDL_Log("Octree Operation Test");
	
	unsigned width = 2048; // Cube side span of 2048 units.
	Octree<unsigned> octree(width); // Create an Octree.
	std::srand(std::time(nullptr)); // Seed random generator.
	for (unsigned i = 0; i < 1000; i++) { // Insert elements at random locations.
		unsigned x = std::rand() % width;
		unsigned y = std::rand() % width;
		unsigned z = std::rand() % width;
		octree.insert(x, y, z, i);
	}
	SDL_Log("Branches: %u\n", octree.countBranches());
	SDL_Log("Leaves: %u\n", octree.countLeaves());
	showNearestNeighbour(octree, 444, 23, 1333);
	return 0;
}