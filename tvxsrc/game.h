#include <glm/glm.hpp>
#include "tvxutil.h"
#include <string.h>

//For sleeping
#include <chrono>
#include <thread>

#define WINDOWS_THREADS //If you're using windows threads

#ifdef WINDOWS_THREADS
#include <windows.h>
#define sleep(n) Sleep(n)
#else
#define sleep(n) std::this_thread::sleep_for(std::chrono::milliseconds(n))
#endif

const unsigned int framedelay = 100; //The inverse of the framerate, in milliseconds!

namespace tvx {


	/* 					Game				 		*/
	/* 	The world basically contains which scene
		is the current. Also, some other 'global'
		settings.									*/

	class Game
	{
	private:
		Scene* current_scene;
		text game_title = strdup("Game");
		glm::ivec2 resolution;

		bool should_stop;
	public:
		Scene& get_current_scene(){return *current_scene;}
		void set_current_scene(Scene* new_scene)
		{
			current_scene = new_scene;

			if(!(current_scene->on_load==nullptr))
				current_scene->on_load(*current_scene);
		}

		text get_title(){return game_title;}
		void set_title(text title){game_title = title;}

		glm::ivec2 get_resolution(){return resolution;}
		void set_resolution(glm::ivec2 res){resolution = res;}
		void set_resolution(int x, int y){resolution = glm::ivec2(x, y);}

		Game():
		resolution(640, 360)
		{}

		void run()
		{
			should_stop = false;
			
			while(!should_stop) {

				current_scene->every_tick(0, *current_scene);
				sleep(framedelay);
			}
		}

		void stop()
		{
			//Close Window
			should_stop = true;
		}
	};
}