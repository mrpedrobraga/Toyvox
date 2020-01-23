#define cls system("cls")

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <glm/glm.hpp>
#include "tvxcore.h"

#include <string>

using namespace tvx;

//Scenes
Scene title = Scene("Title Screen");
Scene world = Scene("My Scene");	

ComponentHandler componentHandler = ComponentHandler();
EntityHandler entityHandler = EntityHandler();

//Init all component sets!
ComponentSet<glm::vec2> Positions2D = ComponentSet<glm::vec2>();
Game TextDungeons = Game();
EntityUID player;

string t1 = "  _____        _     ___                                 \n";
string t2 = " |_   _|____ _| |_  |   \\ _  _ _ _  __ _ ___ ___ _ _  ___\n";
string t3 = "   | |/ -_) \\ /  _| | |) | || | ' \\/ _` / -_) _ \\ ' \\(_-<\n";
string t4 = "   |_|\\___/_\\_\\__| |___/ \\_,_|_||_\\__, \\___\\___/_||_/__/\n";
string t5 = "                                   |___/                 \n";
string t6 = "--------------------------------------------------------------------\n";

void menu_tick(float f, Scene& s)
{
	//TITLE
	std::cout << t1 << t2 << t3 << t4 << t5 << t6;
	std::cout << "S: Start;   C: Config;   Q: Quit!;\n> ";
	switch(std::cin.get()) {
		case 'Q':
			TextDungeons.stop();
			break;
		case 'S':
			cls;
			TextDungeons.set_current_scene(&world);
			break;
	}
	cls;
}

void draw()
{
	glm::ivec2 res = TextDungeons.get_resolution();

	for (int i = 0; i < res.y; ++i)
	{
		for (int j = 0; j < res.x; ++j)
		{		
			if(i == (int)Positions2D.of(player).y && j == (int)Positions2D.of(player).x) {
				std::cout << "#";
			} else {
				std::cout << "O";
			}
		}
		std::cout << "\n";
	}
}

void tick(float f, Scene& s)
{
	draw();
	cls;
}

void init()
{
	title.every_tick = menu_tick;
	world.every_tick = tick;

	TextDungeons.set_resolution(10, 10);
	TextDungeons.set_current_scene(&title);

	world.add_handler(&entityHandler);
	world.add_handler(&componentHandler);

	componentHandler.add("Positions", Positions2D);
}

int main(int argc, char** argv) {

	init();
	TextDungeons.run();
	return 0;
}