
#include "tvxutil.h"
#include "sdldebug.h"

struct Display {
	SDL_Window* window = nullptr;
	SDL_GLContext glcontext = nullptr;
	GLuint window_flags;
	glm::ivec2 resolution = glm::ivec2();

	void draw()
	{
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.3f, 0.8f, 1.5f);
		SDL_GL_SwapWindow(window);
	}

	// FIX APPLIED: use "default arguments" instead of making a ton of different constructors
	explicit Display(
				const char *title = "Toyvox Game",
				glm::ivec2 res = glm::ivec2(640, 360),
				GLuint flags = SDL_WINDOW_SHOWN)
				: window_flags(flags), resolution(res) {
		
		SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
		SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
			fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
			exit(1);
		}

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, CONTEXT_FLAGS);

		window = SDL_CreateWindow(
					title,                    // window title
					SDL_WINDOWPOS_UNDEFINED,  // x (SDL_WINDOWPOS_UNDEFINED means "doesn't matter")
					SDL_WINDOWPOS_UNDEFINED,  // y (SDL_WINDOWPOS_UNDEFINED means "doesn't matter")
					resolution.x,             // width
					resolution.y,             // height
					SDL_WINDOW_OPENGL | flags // flags
		);
		if ( ! window) {
			fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
			exit(1);
		}
		glcontext = SDL_GL_CreateContext(window);
		if ( ! glcontext) {
			fprintf(stderr, "GL Context Creation Failed: %s\n", SDL_GetError());
			exit(9000);
		}

		// Load OpenGL pointers and print info
		printf("OpenGL loaded\n");
		gladLoadGLLoader(SDL_GL_GetProcAddress);
		printf("Vendor:          %s\n", glGetString(GL_VENDOR));
		printf("Renderer:        %s\n", glGetString(GL_RENDERER));
		printf("Version OpenGL:  %s\n", glGetString(GL_VERSION));
		printf("Version GLSL:    %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		
		setupOpenglDebugCallback();
		SDL_GL_SetSwapInterval(1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	~Display()
		{
			SDL_GL_DeleteContext(glcontext);
			SDL_DestroyWindow(window);
		}
};
