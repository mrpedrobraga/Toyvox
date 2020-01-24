#pragma once
#include <glm/glm.hpp>
#include <string.h>
#include "tvxutil.h"
#include "events.h"
#include "SDL2/SDL.h"

//For sleeping
#include <chrono>
#include <thread>

#ifdef WIN32
# define WINDOWS_THREADS //If you're using windows threads
# ifdef WINDOWS_THREADS
#   include <windows.h>
#   define sleep(n) Sleep(n)
# else
#   define sleep(n) std::this_thread::sleep_for(std::chrono::milliseconds(n))
# endif
#endif

namespace tvx {

	const unsigned int framedelay = 100; //The inverse of the framerate, in milliseconds!
	float DELTA_TIME = 0.1;

	/* 					Game				 		*/
	/* 	The world basically contains which scene
		is the current. Also, some other 'global'
		settings.									*/

	class Game
	{
	private:
		Scene* current_scene;
		std::string game_title = strdup("Game");
		glm::ivec2 resolution;
		SDL_Window *window;
		bool should_stop;

		SDL_Event event;
	public:
		Scene& get_current_scene() const{return *current_scene;}
		void set_current_scene(Scene* new_scene)
		{
			current_scene = new_scene;

			if(!(current_scene->on_load==nullptr))
				current_scene->on_load(*current_scene);
		}

		std::string get_title() const{return game_title;}
		void set_title(std::string title){game_title = title;}

		glm::ivec2 get_resolution() const{return resolution;}
		void set_resolution(glm::ivec2 res){resolution = res;}
		void set_resolution(int x, int y){resolution = glm::ivec2(x, y);}

		Game():
		resolution(640, 360)
		{
    		SDL_Init( SDL_INIT_EVERYTHING );
		}

		~Game()
		{
			SDL_DestroyWindow(window);

    		SDL_Quit();
		}

		void run()
		{
			should_stop = false;
			
			window = SDL_CreateWindow(
		        game_title.c_str(),                  // window title
		        SDL_WINDOWPOS_UNDEFINED,           // initial x position
		        SDL_WINDOWPOS_UNDEFINED,           // initial y position
		        resolution.x,                               // width, in pixels
		        resolution.y,                               // height, in pixels
		        SDL_WINDOW_OPENGL                  // flags - see below
		    );

			if (current_scene == 0)
				return;

			while(SDL_WaitEvent(&event)) {
				if(should_stop) break;

				switch(event.type) { //Handle events
					case SDL_KEY_DOWN:
						if (!(current_scene->on_key_pressed == 0)) current_scene->on_key_pressed(event, *current_scene);
						break;
					case SDL_KEY_UP:
						if (!(current_scene->on_key_released == 0)) current_scene->on_key_released(event, *current_scene);
						break;
					case SDL_QUIT:
						stop();
						break;
					default:
						if (!(current_scene->on_event == 0)) current_scene->on_event(event, *current_scene);
				}

				SDL_Delay(framedelay);
			}
		}

		void stop()
		{
			//Close Window
			should_stop = true;
		}
	};

	//Get's a key from an event
	get_key(SDL_Event& e) {
		return e.key.keysym.sym;
	}
}