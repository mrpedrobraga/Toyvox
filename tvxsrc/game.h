#pragma once

#include "events.h"
#include "display.h"
#include "objects.h"

namespace tvx {

	const unsigned int framedelay = 100; //The inverse of the framerate, in milliseconds!
	float DELTA_TIME = 0.1;

	/* 					Game				 		*/
	/* 	The world basically contains which scene
		is the current. Also, some other 'global'
		settings.									*/

	class Game {
		private:

			std::shared_ptr<Scene> current_scene;
			std::string game_title = strdup("Game");
			glm::ivec2 resolution = glm::ivec2();
			SDL_Window *window = nullptr;
			bool should_stop = false;

			SDL_Event event = {};
		public:
			Scene &get_current_scene() const { return *current_scene; }
			void set_current_scene(const std::shared_ptr<Scene> &new_scene) {
				current_scene = new_scene;

				if (current_scene->on_load) {
					current_scene->on_load(*current_scene);
				}
			}

			std::string get_title() const { return game_title; }
			void set_title(std::string title) { game_title = std::move(title); }

			glm::ivec2 get_resolution() const { return resolution; }
			void set_resolution(glm::ivec2 res) { resolution = res; }
			void set_resolution(int x, int y) { resolution = glm::ivec2(x, y); }

			Game() noexcept :
						resolution(640, 360) {
				SDL_Init(SDL_INIT_EVERYTHING);
				window = SDL_CreateWindow(
							game_title.c_str(),                  // window title
							SDL_WINDOWPOS_UNDEFINED,           // initial x position
							SDL_WINDOWPOS_UNDEFINED,           // initial y position
							resolution.x,                               // width, in pixels
							resolution.y,                               // height, in pixels
							SDL_WINDOW_OPENGL                  // flags - see below
				);
			}

			~Game() {
				SDL_DestroyWindow(window);

				SDL_Quit();
			}

			void run() {
				
				while (!should_stop) { // main loop
					
					if (!current_scene) {
						return;
					}
					
					while (SDL_PollEvent(&event)) { // poll each frame

						switch (event.type) { //Handle events
							case SDL_KEYDOWN:
								if (current_scene->on_key_pressed) {
									current_scene->on_key_pressed(event, *current_scene);
								} break;
							case SDL_KEYUP:
								if (current_scene->on_key_released) {
									current_scene->on_key_released(event, *current_scene);
								} break;
							case SDL_QUIT: {
								stop(); // will exit main loop
							} break;
							default: if (current_scene->on_event) { current_scene->on_event(event, *current_scene); }
						}
						SDL_Delay(framedelay);
					}
				}
			}

			void stop() {
				//Close Window
				should_stop = true;
			}
	};

	//Get's a key from an event
	SDL_Keycode get_key(SDL_Event &e) {
		return e.key.keysym.sym;
	}
}