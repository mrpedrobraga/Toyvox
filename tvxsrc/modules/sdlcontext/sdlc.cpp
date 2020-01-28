
#include "sdlc.hpp"

namespace tvx {

#ifdef NDEBUG // Release build
	static constexpr uint32_t CONTEXT_FLAGS = 0;
	static void setupOpenglDebugCallback() { }
#else // Debug Build
	static constexpr uint32_t CONTEXT_FLAGS = SDL_GL_CONTEXT_DEBUG_FLAG;
	// openglCallbackFunction found at https://bcmpinc.wordpress.com/2015/08/21/debugging-since-opengl-4-3/
	static void APIENTRY openglCallbackFunction(
				GLenum source,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const GLchar *message,
				const void *userParam
	) {
		(void) source;
		(void) type;
		(void) id;
		(void) severity;
		(void) length;
		(void) userParam;
		if (severity == GL_DEBUG_SEVERITY_HIGH) {
			SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "%s\n", message);
			exit(102);
		} else {
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "%s\n", message);
		}
	}
	static void setupOpenglDebugCallback() {
		// Enable the debug callback
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglCallbackFunction, nullptr);
		glDebugMessageControl(
					GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true
		);
	}
	static const char *SDL_priority_prefixes[SDL_NUM_LOG_PRIORITIES] = {
				nullptr,
				"VERBOSE",
				"DEBUG",
				"INFO",
				"WARN",
				"ERROR",
				"CRITICAL"
	};
	void SDL_LogOutput(void *userdata, int category, SDL_LogPriority priority, const char *message) {
		if (priority <= SDL_LOG_PRIORITY_INFO) {
			publishf("log", "%s  %s\n", priority == SDL_LOG_PRIORITY_INFO ? "" : SDL_priority_prefixes[priority], message);
		} else {
			publishf("err", "%s  %s\n", SDL_priority_prefixes[priority], message);
		}
	}

#endif

	SdlContext::SdlContext(const char *applicationName)
				: logSub("log", [](void *data) -> void { fprintf(stdout, "%s", (char *) data); }),
				  errSub("err", [](void *data) -> void { fprintf(stderr, "%s", (char *) data); }) {

		SDL_LogSetOutputFunction(SDL_LogOutput, nullptr);
		SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);


		SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s\n", SDL_GetError());
			exit(1);
		}

		// Request an OpenGL context OF THE CORRECT VERSION MATCHING GLAD (should be core) upon window creation
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		// Also request a depth buffer
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		// Request a debug context if debug build active
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, CONTEXT_FLAGS);

		// Windowed, fullscreen desktop (borderless), or dedicated fullscreen
		windowFlags |= SDL_WINDOW_SHOWN; // SDL_WINDOW_SHOWN, SDL_WINDOW_FULLSCREEN_DESKTOP or SDL_WINDOW_FULLSCREEN

		// Create the window
		window = SDL_CreateWindow(
					applicationName,         // window title
					SDL_WINDOWPOS_UNDEFINED, // x (SDL_WINDOWPOS_UNDEFINED means "doesn't matter")
					SDL_WINDOWPOS_UNDEFINED, // y (SDL_WINDOWPOS_UNDEFINED means "doesn't matter")
					1024,                    // width
					512,                     // height
					windowFlags              // flags
		);
		if (!window) {
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL window: %s\n", SDL_GetError());
			exit(1);
		}

		// Create the context
		glContext = SDL_GL_CreateContext(window);
		if (!glContext) {
			SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Fail: %s\n", SDL_GetError());
			exit(1);
		}

		// Load OpenGL pointers and print info
		SDL_Log("OpenGL loading...\n");
		gladLoadGLLoader(SDL_GL_GetProcAddress);
		SDL_Log("Vendor:          %s\n", glGetString(GL_VENDOR));
		SDL_Log("Renderer:        %s\n", glGetString(GL_RENDERER));
		SDL_Log("Version OpenGL:  %s\n", glGetString(GL_VERSION));
		SDL_Log("Version GLSL:    %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		// setup OpenGL debug callbacks if debug build active
		setupOpenglDebugCallback();

		// Use v-sync
		SDL_GL_SetSwapInterval(1);

		// Enable depth test and face culling.
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// Get whatever window size ended up being and setup viewport
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);

		// Initialize the timer
		lastTime = SDL_GetPerformanceCounter();
		toSeconds = 1.0 / (double) SDL_GetPerformanceFrequency();
	}

	SdlContext::~SdlContext() {
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void SdlContext::setFullscreenMode(SDL_WindowFlags mode) {
		SDL_SetWindowFullscreen(window, mode);
	}

# define PUB_KEYD(topic, key) case key: { publish( "key_down_" #topic ); } break;
# define PUB_KEYH(topic, key) if (keyStates[key]) { publish( "key_held_" #topic ); }
# define PUB_MOUD(topic, btn) case btn: { publish( "mouse_down_" #topic ); } break;
# define PUB_MOUH(topic, btn) if (mouseState & SDL_BUTTON(btn)) { publish("mouse_held_" #topic ); }

	/*
	 * returns false if application should exit
	 */
	bool SdlContext::pollEvents() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					return false;
				}
				case SDL_TEXTINPUT:
					if (!SDL_GetRelativeMouseMode()) {
						// placeholder for handling text input
					}
					break;
				case SDL_KEYDOWN: {
					switch (event.key.keysym.scancode) { // Make escape toggle between captured mouse and free mouse
						case SDL_SCANCODE_ESCAPE: {
							if (SDL_GetWindowGrab(window)) {  // window is currently grabbed - ungrab.
								SDL_SetWindowGrab(window, SDL_FALSE);
								SDL_SetRelativeMouseMode(SDL_FALSE);
								publish("window_ungrab");
							} else {  // window is not currently grabbed - grab.
								SDL_SetWindowGrab(window, SDL_TRUE);
								SDL_SetRelativeMouseMode(SDL_TRUE);
								publish("window_grab");
							}
						} break;
						default: break;
					}
					if (SDL_GetRelativeMouseMode()) { // if mouse is captured, handle key down events (placeholder examples)
						switch (event.key.keysym.sym) {
							PUB_KEYD(space, SDLK_SPACE)
							default: break;
						}
					}
				} break;
				case SDL_KEYUP:
					if (SDL_GetRelativeMouseMode()) { // if mouse is captured
						// placeholder for key up events
					}
					break;
				case SDL_MOUSEBUTTONDOWN: {
					switch (event.button.button) {
						PUB_MOUD(left, SDL_BUTTON_LEFT)
						PUB_MOUD(right, SDL_BUTTON_RIGHT)
						PUB_MOUD(middle, SDL_BUTTON_MIDDLE)
						PUB_MOUD(x1, SDL_BUTTON_X1)
						PUB_MOUD(x2, SDL_BUTTON_X2)
						default: break;
					}
				} break;
				case SDL_MOUSEMOTION:
					if (SDL_GetRelativeMouseMode() && (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS)) {
						publish<SDL_Event>("mouse_moved", event);
					} break;
				case SDL_MOUSEWHEEL:
					if (SDL_GetRelativeMouseMode() && (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS)) {
						publish<int32_t>("mouse_wheel", event.wheel.y);
					} break;
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
						case SDL_WINDOWEVENT_SIZE_CHANGED: {
							// placeholder
						} break;
						case SDL_WINDOWEVENT_MOVED: {
							publish("window_moved");
							// placeholder
						} break;
						case SDL_WINDOWEVENT_MAXIMIZED: {
							// placeholder
						} break;
						case SDL_WINDOWEVENT_RESTORED: {
							// placeholder
						} break;
						case SDL_WINDOWEVENT_ENTER: {
							publish("window_entered");
						} break;
						default: break;
					} break;
				default: break;
			}
		}
		return true; // no quit condition detected
	}
	
	/*
	 * This can be connected to a message-passing system or to some sort of observer pattern event system
	 * Knowing the state of keys is more useful and robust than relying on recording up/down events for things like
	 * WASD movement controls
	 */
	void SdlContext::pollStates() {
	
	  if (SDL_GetRelativeMouseMode()) { // if mouse is captured
	    // publish current keyboard state
	    const Uint8 *keyStates = SDL_GetKeyboardState(nullptr);
		  PUB_KEYH(w, SDL_SCANCODE_W)
		  PUB_KEYH(a, SDL_SCANCODE_A)
		  PUB_KEYH(s, SDL_SCANCODE_S)
		  PUB_KEYH(d, SDL_SCANCODE_D)
		  PUB_KEYH(space, SDL_SCANCODE_SPACE)
	
		  // and current mouse state
		  const uint32_t mouseState = SDL_GetMouseState(nullptr, nullptr);
		  PUB_MOUH(left, SDL_BUTTON_LEFT)
		  PUB_MOUH(right, SDL_BUTTON_RIGHT)
		  PUB_MOUH(middle, SDL_BUTTON_MIDDLE)
		  PUB_MOUH(x1, SDL_BUTTON_X1)
		  PUB_MOUH(x2, SDL_BUTTON_X2)
	  }
	}
	
	void SdlContext::setClearColor(float r, float g, float b) {
	  glClearColor(r, g, b, 0.0f);
	}
	
	void SdlContext::clearColor() {
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void SdlContext::swapWindow() {
	  SDL_GL_SwapWindow(window);
	}
	
	float SdlContext::getDeltaTime() {
	  uint64_t currentTime = SDL_GetPerformanceCounter();
	  uint64_t dt = currentTime - lastTime;
	  lastTime = currentTime;
	
	  double dtFloatSeconds = (double)dt * toSeconds;
	
	  // This is a rudimentary framerate calculation that depends on getDeltaTime being called once per frame.
	  frameTime += (float)dtFloatSeconds;
	  ++frameCounter;
	  if (frameTime > 2.f) {
	    float frameTimeAvg = frameTime / (float)frameCounter;
		  SDL_Log("AVG FPS: %.0f\n", 1.f / frameTimeAvg); // print average FPS report every 2 seconds
	    frameTime = 0.f;
	    frameCounter = 0;
	  }
	
	  return (float)dtFloatSeconds; // TODO: should keep double precision?
	}

}
