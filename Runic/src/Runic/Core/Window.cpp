#include "Runic/Core/Window.hpp"

#include "Runic/Core/SDLException.hpp"

namespace Runic
{

Window::Window(WindowData data)
    : _data{std::move(data)}
    , _native(SDL_CreateWindow(
          _data.title.c_str(),
          static_cast<int>(_data.width),
          static_cast<int>(_data.height),
          SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL))
{
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
