#include "Application.hpp"

#include "Runic/Core/Event.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"
#include "SDLException.hpp"
#include "Window.hpp"

#include "SDL3/SDL_init.h"

namespace Runic
{

Application::Application(const AppData& appData, const WindowData& windowData)
{
    Runic::Log::Init(appData.name);

    if (!SDL_Init(SDL_INIT_VIDEO)) throw SDLException("Could not initialize SDL");

    if (!SDL_SetAppMetadata(
            appData.name.c_str(), appData.version.c_str(), appData.identifier.c_str()))
        throw SDLException("Could not set app metadata");

    Window window(windowData);

    _gc = std::make_unique<GraphicsContext>(window);
    _gc->init();
    _gc->setViewport({windowData.width, windowData.height});

    addSystemEventHandler<WindowCloseEvent>(
        [&](const auto& /*event*/)
        {
            _done = true;
            return true;
        });

    addSystemEventHandler<WindowResizeEvent>(
        [&](const WindowResizeEvent& event)
        {
            _gc->setViewport({event.width, event.height});
            return true;
        });

    window.show();
}

} // namespace Runic
