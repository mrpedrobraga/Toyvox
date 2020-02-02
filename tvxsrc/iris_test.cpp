#include <iostream>
#include <Octree.hpp>
#include "native-extensions/iris-renderer/iris-render.h"
#include "tvxutil.h"
#include "tvx_glutil.h"

SDL_Window* window;
SDL_GLContext glcontext;
SDL_Event event;

struct Color {
private:
	unsigned char mr, mg, mb, ma;
public:

	Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a): mr(_r), mg(_g), mb(_b), ma(_a) {}

	unsigned char& r() {return mr;}
	unsigned char& g() {return mg;}
	unsigned char& b() {return mb;}
	unsigned char& a() {return ma;}

	void set(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a){mr = _r; mg = _g; mb = _b; ma = _a;}
};
Octree<Color> model(2048);

bool quit = false;

#define ERRORCHECK while((err = glGetError()) != GL_NO_ERROR){ std::cout << "Error here"; }
#define HEYTHERE std::cout << "Well this is supposed to be printed on the console."
GLenum err;

void generate_points(GLuint &AO, GLuint &BO) {
	float vertices[] =
	{
			-0.5, -0.5, 0.0,
			+0.5, -0.5, 0.0,
			+0.5, +0.5, 0.0,
			-0.5, +0.5, 0.0
	};
	glGenVertexArrays(1, &AO);
	ERRORCHECK;
	glGenBuffers(1, &BO);
	ERRORCHECK;
	glBindVertexArray(AO);
	ERRORCHECK;
	glBindBuffer(GL_ARRAY_BUFFER, BO);
	ERRORCHECK;
	{
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW );
		ERRORCHECK;
		glEnableVertexAttribArray( 0 );
		ERRORCHECK;
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
		ERRORCHECK;
	}
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	ERRORCHECK;
	glBindVertexArray( 0 );
	ERRORCHECK;
}

int main (int argc, char** argv) {
	HEYTHERE;
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_LoadLibrary(NULL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	//Request Debug
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	window = SDL_CreateWindow("Toyvox Game",
														SDL_WINDOWPOS_UNDEFINED,
														SDL_WINDOWPOS_UNDEFINED,
														960,
														540,
														SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	glcontext = SDL_GL_CreateContext(window);

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	SDL_GL_SetSwapInterval(1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	SDL_SetWindowGrab(window, SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	HEYTHERE;
	GLuint program = tvx::program_from_file("native-extensios/iris-renderer/main.vert", "native-extensions/iris-renderer/main.frag");
	if (!program) { SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Couldn't compile shaders!!!\n"); }
	ERRORCHECK;
	glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
	GLuint AO, BO;
	generate_points(AO, BO);

	while (!quit) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ERRORCHECK;
		glUseProgram(program);
		ERRORCHECK;
		glBindVertexArray(AO);
		ERRORCHECK;
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		ERRORCHECK;
		glBindVertexArray(0);
		ERRORCHECK;
	  glUseProgram(0);
		ERRORCHECK;

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
