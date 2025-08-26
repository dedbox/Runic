#pragma once

#include "SDL3/SDL_video.h"

#include "Runic/Core/Window.hpp"

namespace Runic
{

class GraphicsContext
{
public:
    explicit GraphicsContext(const Window& window);

    void init();

    Window& getWindow() { return _window; }

private:
    Window _window;
    SDL_GLContext _native{nullptr};
};

} // namespace Runic
