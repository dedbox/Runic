#pragma once

#include "Runic/Core/Application.hpp"

#define SDL_MAIN_USE_CALLBACKS 1

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_main.h"

inline SDL_AppResult SDL_AppInit(void** appstate, int /*argc*/, char** /*arg*/)
{
    std::unique_ptr<Runic::Application> app{Runic::CreateApplication()};

    *appstate = app.release();

    return SDL_APP_CONTINUE;
}

inline SDL_AppResult SDL_AppIterate(void* /*appstate*/)
{
    return SDL_APP_CONTINUE;
}

inline SDL_AppResult SDL_AppEvent(void* /*appstate*/, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;

    return SDL_APP_CONTINUE;
}

inline void SDL_AppQuit(void* appstate, SDL_AppResult /*result*/)
{
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    delete static_cast<Runic::Application*>(appstate);
}
