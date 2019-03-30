
#pragma once

#include <glad/glad.h>
#include <SDL_video.h>
#include <SDL_version.h>

#if !SDL_VERSION_ATLEAST(2, 0, 8)
#error "SDL2 version 2.0.8 or newer required."
#endif

namespace tvx {
  class SdlContext {
    public:
      explicit SdlContext(const char *applicationName);
      virtual ~SdlContext();
      void setFullscreenMode(SDL_WindowFlags mode);
      bool pollEvents();
      void pollStates();
      void clearColor();
      void swapWindow();
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
  };
}
