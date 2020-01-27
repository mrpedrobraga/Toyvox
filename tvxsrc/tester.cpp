
#include "tester.h"

using namespace std;
using namespace tvx;

using namespace tvx;

Game testGame = Game();
EntityUID player;

void tick_func(float f, Scene& s) {

	Positions2D.apply([](glm::vec2& p){
		p.y += 0.1;
	});

	std::cout << "Player at " << Positions2D.of(player).x << ", " << Positions2D.of(player).y << ".\n";
}

void init() {

	scene1.every_tick = tick_func;

	testGame.set_resolution(640, 360);
	testGame.set_current_scene(&scene1);

	scene1.add_handler(&entityHandler);
	scene1.add_handler(&componentHandler);

	componentHandler.add("Positions", Positions2D);
}

int main(int argc, char* argv[])
{
	std::cin.get();
	init();
	player = testGame.get_current_scene().create_entity();
	testGame.run();

	getchar();
	return 0;
}