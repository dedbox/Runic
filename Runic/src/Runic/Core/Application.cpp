#include "Application.hpp"

#include "SDLException.hpp"

#include "SDL3/SDL_init.h"

namespace Runic
{

Application::Application(const AppData& data, const WindowData& windowData)
    : _data{data}
{
    if (!SDL_SetAppMetadata(data.name.c_str(), data.version.c_str(), data.identifier.c_str()))
        throw SDLException("Could not set app metadata");

    if (!SDL_Init(SDL_INIT_VIDEO))
        throw SDLException("Could not initialize SDL");

    _window = std::make_unique<Window>(windowData);
    _window->show();
}

} // namespace Runic
