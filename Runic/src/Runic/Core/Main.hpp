#pragma once

#define SDL_MAIN_USE_CALLBACKS 1

#include "Application.hpp"
#include "Event.hpp"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_main.h"
#include "backends/imgui_impl_sdl3.h"

inline SDL_AppResult SDL_AppInit(void** appstate, int /*argc*/, char** /*arg*/)
{
    std::unique_ptr<Runic::Application> app = Runic::CreateApplication();

    *appstate = app.release();

    return SDL_APP_CONTINUE;
}

inline SDL_AppResult SDL_AppIterate(void* appstate)
{
    auto app = static_cast<Runic::Application*>(appstate);

    if (app->isDone())
        return SDL_APP_SUCCESS;

    app->onUpdate();

    return SDL_APP_CONTINUE;
}

inline SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (ImGui::GetCurrentContext() && ImGui_ImplSDL3_ProcessEvent(event))
        return SDL_APP_CONTINUE;

    auto app = static_cast<Runic::Application*>(appstate);

    switch (event->type)
    {
        // Window Events ---------------------------------------------------------------------------

    case SDL_EVENT_QUIT:
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        app->dispatchEvent(Runic::WindowCloseEvent());
        break;

    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        app->dispatchEvent(Runic::WindowFocusEvent());
        break;

    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
        float scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        int x = 0, y = 0;
        SDL_GetWindowSizeInPixels(app->getWindow()->getNative(), &x, &y);
        app->dispatchEvent(Runic::WindowResizeEvent(x * scale, y * scale));
        break;
    }

    case SDL_EVENT_WINDOW_FOCUS_LOST:
        app->dispatchEvent(Runic::WindowUnfocusEvent());
        break;

        // Key Events ------------------------------------------------------------------------------

    case SDL_EVENT_KEY_DOWN: {
        Runic::Key key{static_cast<Runic::Key>(event->key.key)};
        app->dispatchEvent(Runic::KeyPressEvent(key, event->key.repeat));
        break;
    }

    case SDL_EVENT_KEY_UP:
        app->dispatchEvent(Runic::KeyReleaseEvent(static_cast<Runic::Key>(event->key.key)));
        break;

        // Mouse Events ----------------------------------------------------------------------------

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        app->dispatchEvent(
            Runic::MouseButtonPressEvent(
                static_cast<Runic::MouseButton>(event->button.button),
                event->button.x,
                event->button.y));
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
        app->dispatchEvent(
            Runic::MouseButtonReleaseEvent(
                static_cast<Runic::MouseButton>(event->button.button),
                event->button.x,
                event->button.y));
        break;

    case SDL_EVENT_MOUSE_MOTION:
        app->dispatchEvent(Runic::MouseMoveEvent(event->motion.xrel, event->motion.yrel));
        break;

    case SDL_EVENT_MOUSE_WHEEL:
        app->dispatchEvent(Runic::MouseScrollEvent(event->wheel.x, event->wheel.y));
        break;

    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

inline void SDL_AppQuit(void* appstate, SDL_AppResult /*result*/)
{
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    delete static_cast<Runic::Application*>(appstate);
}
