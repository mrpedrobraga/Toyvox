
#pragma once

#include <SDL_video.h>
#include <SDL_version.h>

#if !SDL_VERSION_ATLEAST(2, 0, 8)
#error "SDL2 version 2.0.8 or newer required."
#endif

namespace toyvox {
  class SdlContext {
    public:
      explicit SdlContext(const char *applicationName);
      SDL_Window * getWindow();
      void setFullscreenMode(SDL_WindowFlags mode);
      bool pollEvents();
      void pollStates();
    private:
      SDL_Window *window = nullptr;
      uint32_t windowFlags = 0;

      uint64_t lastTime = 0;
      double toSeconds = 0;

      uint32_t frameCounter = 0;
      float frameTime = 0;

      static char *clipboardData;
  };
}
