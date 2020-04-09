#pragma once

#include "tvxutil.h"
#include "events.h"
#include "objects.h"
#include "display.h"

namespace tvx::game {

	const unsigned int framedelay = 100; //The inverse of the framerate, in milliseconds!
	float DELTA_TIME = 0.1;

	std::shared_ptr<Scene> current_scene;
	std::string game_title = strdup("Game");
	bool should_stop = false;
	SDL_Window* window;
	SDL_GLContext glcontext;
	GLuint window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	glm::ivec2 resolution = glm::ivec2(160, 90);
	Display current_display(game_title, resolution, window_flags);

	Scene &get_current_scene() { return *current_scene; }
	void set_current_scene(std::shared_ptr<Scene>& new_scene) {
		current_scene = (new_scene);

		if (current_scene->on_load) {
			current_scene->on_load(*current_scene);
		}
	}

	std::string get_title() { return game_title; }
	void set_title(std::string title) { game_title = strdup(title.c_str()); }

	void init() {
		current_display.init();
	}

	void run() {

		while( !(current_display).is_closed() && !should_stop )
		{
				glClearColor( 0.2f, 0.4f, 0.8f, 1.0f );
				glClear( GL_COLOR_BUFFER_BIT );
				(current_display).update(current_scene);
		}
	}

	void stop() {
		//Close Window
		should_stop = true;
	}

	//Get's a key from an event
	SDL_Keycode get_key(SDL_Event &e) {
		return e.key.keysym.sym;
	}

	glm::ivec2 get_resolution() { return resolution; }
	void set_resolution(glm::ivec2 res) { resolution = res; }
	void set_resolution(int x, int y) { resolution = glm::ivec2(x, y); }
}
