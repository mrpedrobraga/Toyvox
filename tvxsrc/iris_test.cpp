#include <iostream>
#include <Octree.hpp>
#include "native-extensions/iris-renderer/iris-render.h"
#include "tvxutil.h"
#include "tvx_glutil.h"

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

void generate_points(GLuint &AO, GLuint &BO) {
	float vertices[] =
	{
			-0.5, -0.5, 0.0,
			+0.5, -0.5, 0.0,
			+0.5, +0.5, 0.0,
			-0.5, +0.5, 0.0
	};
	glGenVertexArrays(1, &AO);
		glGenBuffers(1, &BO);
		glBindVertexArray(AO);
		glBindBuffer(GL_ARRAY_BUFFER, BO);
		{
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW );
				glEnableVertexAttribArray( 0 );
				glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
			}
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindVertexArray( 0 );
	}

int main (int argc, char** argv) {

	SDL_Window* window;
	SDL_GLContext glcontext;
	SDL_Event event;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_LoadLibrary(NULL);

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	//Request Debug
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	window = SDL_CreateWindow("Toyvox Game",
														SDL_WINDOWPOS_UNDEFINED,
														SDL_WINDOWPOS_UNDEFINED,
														320,
														180,
														SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL window: %s\n", SDL_GetError()); fflush(stdout);fflush(stderr);
	}

	glcontext = SDL_GL_CreateContext(window);
	if (!glcontext) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Fail: %s\n", SDL_GetError());fflush(stdout);fflush(stderr);
	}

	// Load OpenGL pointers and print info
	printf("OpenGL loading...\n");

	printf("Vendor:          %s\n", glGetString(GL_VENDOR));
	printf("Renderer:        %s\n", glGetString(GL_RENDERER));
	printf("Version OpenGL:  %s\n", glGetString(GL_VERSION));
	printf("Version GLSL:    %s\n\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	fflush(stdout); fflush(stderr);

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	SDL_GL_SetSwapInterval(1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//SDL_SetWindowGrab(window, SDL_TRUE);
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	const char* vertex = 		"#version 330 core\n"
													"layout (location = 0) in vec3 vpos\n"
													"void main(){\n"
													"gl_Position = vec4(vpos, 1.0);"
													"}";

	const char* fragment = 	"#version 330 core\n"
													"void main(){\n"
													"gl_FragColor = vec4(0.2, 0.4, 0.8, 1.0);\n"
													"}";

  //std::cout << vertex << "\n\n" << fragment << "\n\nThose are the shaders!\n"; fflush(stdout);

	GLuint program = tvx::program_from_string(vertex, fragment);

	if (!program) { SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Couldn't compile shaders!!!\n"); fflush(stderr); }
	glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
	//GLuint AO, BO;
	//generate_points(AO, BO);

	while (!quit) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				//glUseProgram(program);
				//glBindVertexArray(AO);
				//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				//glBindVertexArray(0);
			  //glUseProgram(0);

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
