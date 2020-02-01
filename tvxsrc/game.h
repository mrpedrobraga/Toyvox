#pragma once

#include "tvxutil.h"
#include "tvx_glutil.h"
#include "events.h"
#include "objects.h"

namespace tvx::game {

	const unsigned int framedelay = 100; //The inverse of the framerate, in milliseconds!
	float DELTA_TIME = 0.1;

	std::shared_ptr<Scene> current_scene;
	std::string game_title = strdup("Game");
	bool should_stop = false;
	SDL_Window* window;
	SDL_GLContext glcontext;
	GLuint window_flags = SDL_WINDOW_OPENGL;
	glm::ivec2 resolution = glm::ivec2(640, 360);
	SDL_Event event = {};

	Scene &get_current_scene() { return *current_scene; }
	void set_current_scene(std::shared_ptr<Scene> &new_scene) {
		current_scene = std::make_shared<Scene>(*new_scene);

		if (current_scene->on_load) {
			current_scene->on_load(*current_scene);
		}
	}

	std::string get_title() { return game_title; }
	void set_title(std::string title) { game_title = std::move(title); }

	void run() {
		//SDL_CreateThread(tick, "Tick", (void*) NULL);
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

	void initSDLGL() {

		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_LoadLibrary(NULL);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	}

	void draw()
	{

		GLuint program = LoadShaders("native-extensions/iris-renderer/main.vert", "native-extensions/iris-renderer/main.frag");

		while(!should_stop)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.1f, 0.3f, 0.8f, 1.5f);
		  SDL_GL_SwapWindow(window);

			if(current_scene->every_tick) current_scene->every_tick(0.0, *current_scene);

			SDL_Delay(framedelay);

			while(SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT:
						stop();
						break;
					default:
						if(current_scene->on_event) current_scene->on_event(event, *current_scene);
						break;
				}
			}
		}

		SDL_GL_DeleteContext(glcontext);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void startGLContext()
	{
		glcontext = SDL_GL_CreateContext(window);
		gladLoadGLLoader(SDL_GL_GetProcAddress);

		SDL_GL_SetSwapInterval(1);

		draw();
	}

	void launch()
		{
			initSDLGL();
			window = SDL_CreateWindow(
						game_title.c_str(),                  // window title
						SDL_WINDOWPOS_UNDEFINED,           // initial x position
						SDL_WINDOWPOS_UNDEFINED,           // initial y position
						resolution.x,                               // width, in pixels
						resolution.y,                               // height, in pixels
						window_flags                  // flags - see below
			); startGLContext();
		}

	void launch(GLuint flags)
		{
			initSDLGL();
			window = SDL_CreateWindow(
						game_title.c_str(),                  // window title
						SDL_WINDOWPOS_UNDEFINED,           // initial x position
						SDL_WINDOWPOS_UNDEFINED,           // initial y position
						resolution.x,                               // width, in pixels
						resolution.y,                               // height, in pixels
						SDL_WINDOW_OPENGL | flags                   // flags - see below
			); startGLContext();
		}
}
