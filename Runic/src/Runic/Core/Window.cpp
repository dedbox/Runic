#include "Runic/Core/Window.hpp"

#include "SDL3/SDL_video.h"

namespace Runic
{

Window::Window(const WindowData& data)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    if constexpr (Core::Debugging)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    }

    _native = SDL_CreateWindow(
        data.title.c_str(), static_cast<int>(data.width), static_cast<int>(data.height),
        SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if (!_native) throw SDLException("Could not create window");
}

void Window::show() const
{
    if (!SDL_ShowWindow(_native)) throw SDLException("Could not show window");
}

void Window::hide() const
{
    if (!SDL_HideWindow(_native)) throw SDLException("Could not hide window");
}

} // namespace Runic
