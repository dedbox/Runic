#pragma once

#define SDL_MAIN_USE_CALLBACKS 1

#include "SDL3/SDL_main.h"

inline SDL_AppResult SDL_AppInit(void** appstate, int argc, char** arg) {
  return SDL_APP_CONTINUE;
}

inline SDL_AppResult SDL_AppIterate(void* appstate) {
  return SDL_APP_CONTINUE;
}

inline SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  return SDL_APP_CONTINUE;
}

inline void SDL_AppQuit(void* appstate, SDL_AppResult result) {}
