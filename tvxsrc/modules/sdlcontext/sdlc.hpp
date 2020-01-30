
#pragma once

#include "extern.hpp"
#include "topics.hpp"

#if !SDL_VERSION_ATLEAST(2, 0, 8)
#error "SDL2 version 2.0.8 or newer required."
#endif

namespace tvx {
  class SdlContext {
    public:
      explicit SdlContext(const char *applicationName);
      virtual ~SdlContext();
      void setFullscreenMode(SDL_WindowFlags mode);
      bool pollEvents(bool isQuitRequested);
      static void pollStates();
      void setClearColor(float r, float g, float b);
      void clearColor();
      void swapWindow();
      float getDeltaTime();
    private:
      SDL_Window *window = nullptr;
      uint32_t windowFlags = SDL_WINDOW_OPENGL;
      int windowWidth = 0;
      int windowHeight = 0;
      SDL_GLContext glContext = nullptr;

      uint64_t lastTime = 0;
      double toSeconds = 0;

      uint32_t frameCounter = 0;
      float frameTime = 0;
      
      Subscription logSub, errSub;
  };
}
