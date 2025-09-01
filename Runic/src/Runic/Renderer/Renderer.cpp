#include "Renderer.hpp"

namespace Runic
{

Renderer::Renderer(SDL_Window* window)
    : _window(window)
{
}

void Renderer::beginFrame() {}

void Renderer::endFrame()
{
    SDL_GL_SwapWindow(_window);
}

} // namespace Runic
