
#include <cstdio>
#include <SDL.h>
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
        const GLchar* message,
        const void* userParam
  ){
    (void)source; (void)type; (void)id;
    (void)severity; (void)length; (void)userParam;
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
#endif

SdlContext::SdlContext(const char *applicationName) {

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
  if ( ! window) {
	  SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL window: %s\n", SDL_GetError());
    exit(1);
  }

  // Create the context
  glContext = SDL_GL_CreateContext(window);
  if ( ! glContext) {
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
  toSeconds = 1.0 / (double)SDL_GetPerformanceFrequency();
}

SdlContext::~SdlContext() {
  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void SdlContext::setFullscreenMode(SDL_WindowFlags mode) {
  SDL_SetWindowFullscreen(window, mode);
}

/*
 * returns false if application should exit
 */
bool SdlContext::pollEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT: {
        return false;
      }
      case SDL_TEXTINPUT:
        if ( ! SDL_GetRelativeMouseMode()) {
          // placeholder for handling text input
        } break;
      case SDL_KEYDOWN: {
        switch (event.key.keysym.scancode) { // Make escape toggle between captured mouse and free mouse
          case SDL_SCANCODE_ESCAPE: {
            if(SDL_GetWindowGrab(window)) {  // window is currently grabbed - ungrab.
              SDL_SetWindowGrab(window, SDL_FALSE);
              SDL_SetRelativeMouseMode(SDL_FALSE);
            } else {  // window is not currently grabbed - grab.
              SDL_SetWindowGrab(window, SDL_TRUE);
              SDL_SetRelativeMouseMode(SDL_TRUE);
            }
          } break;
          default: break;
        }
        if (SDL_GetRelativeMouseMode()) { // if mouse is captured, handle key down events (placeholder examples)
          switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_F1: break;
            case SDL_SCANCODE_F2: break;
            case SDL_SCANCODE_F3: break;
            case SDL_SCANCODE_F4: break;
            case SDL_SCANCODE_F5: break;
            case SDL_SCANCODE_F6: break;
            case SDL_SCANCODE_F7: break;
            case SDL_SCANCODE_F8: break;
            case SDL_SCANCODE_F9: break;
            case SDL_SCANCODE_F10: break;
            case SDL_SCANCODE_F11: break;
            case SDL_SCANCODE_F12: break;

            case SDL_SCANCODE_0: break;
            case SDL_SCANCODE_1: break;
            case SDL_SCANCODE_2: break;
            case SDL_SCANCODE_3: break;
            case SDL_SCANCODE_4: break;
            case SDL_SCANCODE_5: break;
            case SDL_SCANCODE_6: break;
            case SDL_SCANCODE_7: break;
            case SDL_SCANCODE_8: break;
            case SDL_SCANCODE_9: break;
            default: break;
          }
        }
      } break;
      case SDL_KEYUP:
        if (SDL_GetRelativeMouseMode()) { // if mouse is captured
          // placeholder for key up events
        }
      case SDL_MOUSEBUTTONDOWN:
        if (SDL_GetRelativeMouseMode()) { // if mouse is captured, handle button presses (placeholder examples)
          switch(event.button.button) {
            case SDL_BUTTON_LEFT: break;
            case SDL_BUTTON_RIGHT: break;
            case SDL_BUTTON_MIDDLE: break;
            case SDL_BUTTON_X1: break;
            case SDL_BUTTON_X2: break;
            default: break;
          }
        }
      case SDL_MOUSEMOTION:
        if (SDL_GetRelativeMouseMode()) { // if mouse is captured
          // placeholder for mouse motion events
        } break;
      case SDL_MOUSEWHEEL:
        if (SDL_GetRelativeMouseMode()) { // if mouse is captured
          // placeholder for mouse wheel scroll events
        } break;
      case SDL_WINDOWEVENT:
        switch (event.window.event) {
          case SDL_WINDOWEVENT_SIZE_CHANGED: {
            // placeholder
          } break;
          case SDL_WINDOWEVENT_MOVED: {
            // placeholder
          } break;
          case SDL_WINDOWEVENT_MAXIMIZED: {
            // placeholder
          } break;
          case SDL_WINDOWEVENT_RESTORED: {
            // placeholder
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
    const Uint8 *keyStates = SDL_GetKeyboardState(NULL);
    if (keyStates[SDL_SCANCODE_W]) { /*W is currently pressed*/ }
    if (keyStates[SDL_SCANCODE_A]) { /*A is currently pressed*/ }
    if (keyStates[SDL_SCANCODE_S]) { /*S is currently pressed*/ }
    if (keyStates[SDL_SCANCODE_D]) { /*D is currently pressed*/ }

    // publish current mouse state
    const uint32_t mouseState = SDL_GetMouseState(nullptr, nullptr);
    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) { /*LEFT button is currently held*/ }
    if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) { /*RIGHT button is currently held*/ }
    if (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)) { /*MIDDLE button is currently held*/ }
    if (mouseState & SDL_BUTTON(SDL_BUTTON_X1)) { /*X1 button is currently held*/ }
    if (mouseState & SDL_BUTTON(SDL_BUTTON_X2)) { /*X2 button is currently held*/ }
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
