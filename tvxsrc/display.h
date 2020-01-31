
#include "tvxutil.h"

struct Display {
	SDL_Window* window;
	SDL_GLContext glcontext;
	GLuint window_flags;
	glm::ivec2 resolution = glm::ivec2();

	void startGLContext()
	{
		glcontext = SDL_GL_CreateContext(window);
	}

	void draw()
	{
		glClearColor(1, 1, 1, 1);
	}

	Display ():
		window_flags(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE), resolution(glm::ivec2(640, 360))
		{
			window = SDL_CreateWindow(
						"Toyvox Game",                  // window title
						SDL_WINDOWPOS_UNDEFINED,           // initial x position
						SDL_WINDOWPOS_UNDEFINED,           // initial y position
						resolution.x,                               // width, in pixels
						resolution.y,                               // height, in pixels
						window_flags                  // flags - see below
			); startGLContext();
		}

	Display (GLuint flags):
		window_flags(SDL_WINDOW_OPENGL | flags), resolution(glm::ivec2(640, 360))
		{
			window = SDL_CreateWindow(
						"Toyvox Game",                  // window title
						SDL_WINDOWPOS_UNDEFINED,           // initial x position
						SDL_WINDOWPOS_UNDEFINED,           // initial y position
						resolution.x,                               // width, in pixels
						resolution.y,                               // height, in pixels
						window_flags                  // flags - see below
			); startGLContext();
		}

	Display (string& title, GLuint flags):
		window_flags(SDL_WINDOW_OPENGL | flags), resolution(glm::ivec2(640, 360))
		{
			window = SDL_CreateWindow(
						title.c_str(),                  // window title
						SDL_WINDOWPOS_UNDEFINED,           // initial x position
						SDL_WINDOWPOS_UNDEFINED,           // initial y position
						resolution.x,                               // width, in pixels
						resolution.y,                               // height, in pixels
						window_flags                  // flags - see below
			); startGLContext();
		}

	Display (string& title, glm::ivec2 res, GLuint flags):
		window_flags(SDL_WINDOW_OPENGL | flags), resolution(res)
		{
			window = SDL_CreateWindow(
						title.c_str(),                  // window title
						SDL_WINDOWPOS_UNDEFINED,           // initial x position
						SDL_WINDOWPOS_UNDEFINED,           // initial y position
						resolution.x,                               // width, in pixels
						resolution.y,                               // height, in pixels
						window_flags                  // flags - see below
			); startGLContext();
		}

	~Display()
		{
			SDL_GL_DeleteContext(glcontext);
			SDL_DestroyWindow(window);
		}
};
