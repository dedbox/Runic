#pragma once

#include <SDL3/SDL_video.h>

namespace Runic
{

class Renderer
{
public:
    explicit Renderer(SDL_Window* window);

    void beginFrame();
    void endFrame();

private:
    SDL_Window* _window;
};

} // namespace Runic
