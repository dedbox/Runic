#include "Runic/Core/Window.hpp"

#include <SDL3/SDL_mouse.h>

namespace Runic
{

Window::Window(const WindowData& data)
    : _resizePolicy(data.resizePolicy)
    , _viewportSize(data.width, data.height)
    , _viewportOffset(0, 0)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    if constexpr (Core::Debugging)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    }

    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

    _native = SDL_CreateWindow(
        data.title.c_str(),
        static_cast<int>(static_cast<float>(data.width) * main_scale),
        static_cast<int>(static_cast<float>(data.height) * main_scale),
        SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL |
            SDL_WINDOW_HIGH_PIXEL_DENSITY);

    if (!_native)
        throw SDLException("Could not create window");
}

void Window::show() const
{
    if (!SDL_ShowWindow(_native))
        throw SDLException("Could not show window");
}

void Window::hide() const
{
    if (!SDL_HideWindow(_native))
        throw SDLException("Could not hide window");
}

void Window::setSize(const glm::uvec2& size)
{
    std::visit(
        [&](auto&& policy) {
            using T = std::decay_t<decltype(policy)>;
            if constexpr (std::is_same_v<T, AdaptiveResize>)
                _viewportSize = size;
            else if constexpr (std::is_same_v<T, FixedAspect>)
            {
                _viewportSize.x = static_cast<int>(size.x);
                _viewportSize.y =
                    static_cast<int>(static_cast<float>(_viewportSize.x) / policy.aspectRatio);

                if (_viewportSize.y > size.y)
                {
                    _viewportSize.y = static_cast<int>(size.y);
                    _viewportSize.x =
                        static_cast<int>(static_cast<float>(_viewportSize.y) * policy.aspectRatio);
                }

                _viewportOffset.x = (size.x - _viewportSize.x) / 2;
                _viewportOffset.y = (size.y - _viewportSize.y) / 2;
            }
        },
        _resizePolicy);
}

const glm::ivec2 Window::getSize() const
{
    int width  = 0;
    int height = 0;
    if (!SDL_GetWindowSizeInPixels(_native, &width, &height))
        throw SDLException("Could not get window size");

    return {width, height};
}

void Window::captureMouse() const
{
    SDL_SetWindowRelativeMouseMode(_native, true);
}

void Window::releaseMouse() const
{
    SDL_SetWindowRelativeMouseMode(_native, false);
}

bool Window::isMouseCaptured() const
{
    return SDL_GetWindowRelativeMouseMode(_native);
}

} // namespace Runic
