#pragma once

#include "tvxutil.h"
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
			std::string game_title = "Game";
			glm::ivec2 resolution;
			std::shared_ptr<Display> display; // FIX APPLIED: ptr to avoid accidental (de-)constructor calls
			bool should_stop = false;
			
		public:
			Scene &get_current_scene() const { return *current_scene; }
			void set_current_scene(const std::shared_ptr<Scene> &new_scene) {
				current_scene = new_scene;

				if (current_scene->on_load) {
					current_scene->on_load(*current_scene);
				}
			}

			Display const &get_current_display() { return *display; }
			void set_current_display(const std::shared_ptr<Display> &new_display) {
				display = new_display;
			}
			
			std::string get_title() const { return game_title; }
			void set_title(std::string title) { game_title = std::move(title); }

			glm::ivec2 get_resolution() const { return display->resolution; }
			void set_resolution(glm::ivec2 res) { resolution = res; display->resolution = res; }
			void set_resolution(int x, int y) { resolution = glm::ivec2(x, y); display->resolution = glm::ivec2(x, y); }

			Game():
				resolution(640, 360)
			{
				display = std::make_shared<Display>(game_title.c_str(), resolution);
			}

			~Game() {

				SDL_Quit();
			}

			void run() {
				
				while (!should_stop) { // main loop

					if (display) { display->draw(); }
					
					if (!current_scene) {
						return;
					}
					
					SDL_Event event;
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