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


	GLuint VertexArrayID;
	GLuint vertexbuffer;

	void drawQuad() {

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
		   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   0,                  // stride
		   (void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);
	}

	void draw()
	{

		GLuint program = LoadShaders("native-extensions/iris-renderer/main.vert", "native-extensions/iris-renderer/main.frag");

		glClearColor(0.1f, 0.3f, 0.8f, 1.5f);
		glUseProgram(program);

		while(!should_stop)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.1f, 0.3f, 0.8f, 1.5f);

			drawQuad();

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

		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		static const GLfloat g_vertex_buffer_data[] = {
		   -1.0f, -1.0f, 0.0f,
		   1.0f, -1.0f, 0.0f,
		   0.0f,  1.0f, 0.0f,
		};
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexbuffer);
		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

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
