
#include "sdlc.h"
#include <SDL.h>
#include <stdio.h>

namespace toyvox {

  SdlContext::SdlContext(const char *applicationName) {

    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
    SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
      fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    }

    // Windowed, fullscreen desktop (borderless), or dedicated fullscreen
    windowFlags |= SDL_WINDOW_RESIZABLE;
    // windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    // windowFlags |= SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(
        applicationName,         // window title
        SDL_WINDOWPOS_UNDEFINED, // x (SDL_WINDOWPOS_UNDEFINED means "doesn't matter")
        SDL_WINDOWPOS_UNDEFINED, // y (SDL_WINDOWPOS_UNDEFINED means "doesn't matter")
        1024,                    // width
        768,                     // height
        windowFlags              // flags
    );
    if (window == nullptr) {
      fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
    }
  }

  SDL_Window * SdlContext::getWindow() {
    return window;
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
}
