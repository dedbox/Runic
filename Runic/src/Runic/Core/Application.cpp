#include "Application.hpp"

#include "Log.hpp"
#include "Runic/Core/Event.hpp"
#include "SDLException.hpp"

#include "SDL3/SDL_init.h"

namespace Runic
{

Application::Application(const AppData& data, const WindowData& windowData)
    : _data{data}
{
    Runic::Log::Init(_data.name);

    if (!SDL_SetAppMetadata(data.name.c_str(), data.version.c_str(), data.identifier.c_str()))
        throw SDLException("Could not set app metadata");

    if (!SDL_Init(SDL_INIT_VIDEO)) throw SDLException("Could not initialize SDL");

    _window = std::make_unique<Window>(windowData);

    addSystemEventHandler<WindowCloseEvent>(
        [&](const auto& /*event*/)
        {
            _done = true;
            return true;
        });

    _context = std::make_unique<GraphicsContext>(_window.get());
    _context->setViewport({windowData.width, windowData.height});

    addSystemEventHandler<WindowResizeEvent>(
        [&](const WindowResizeEvent& event)
        {
            _context->setViewport({event.width, event.height});
            return true;
        });

    _window->show();
}

} // namespace Runic
