#include <iostream>
#include "tvxutil.h"

SDL_Window* window;
SDL_GLContext glcontext;
SDL_Event event;

bool quit = false;

int main (int argc, char** argv) {

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_LoadLibrary(NULL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	window = SDL_CreateWindow("Toyvox Game",
														SDL_WINDOWPOS_UNDEFINED,
														SDL_WINDOWPOS_UNDEFINED,
														960,
														540,
														SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	glcontext = SDL_GL_CreateContext(window);

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	SDL_GL_SetSwapInterval(1);

	while (!quit) {
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.3f, 0.8f, 1.5f);
	  SDL_GL_SwapWindow(window);
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT) quit = true;
			if(event.type == SDL_KEYDOWN)
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = true;
						break;
				}
		}
	}

	SDL_Quit();
}
