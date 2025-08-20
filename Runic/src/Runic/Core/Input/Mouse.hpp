#pragma once

#include "SDL3/SDL_mouse.h"

namespace Runic
{

enum class MouseButton : uint8_t
{
    Left   = SDL_BUTTON_LEFT,
    Middle = SDL_BUTTON_MIDDLE,
    Right  = SDL_BUTTON_RIGHT,
    X1     = SDL_BUTTON_X1,
    X2     = SDL_BUTTON_X2,
};

inline auto format_as(const MouseButton& button)
{
    switch (button)
    {
    case MouseButton::Left: return "Left";
    case MouseButton::Middle: return "Middle";
    case MouseButton::Right: return "Right";
    case MouseButton::X1: return "X1";
    case MouseButton::X2: return "X2";
    }
    return "unknown";
}

inline const MouseButton operator|(const MouseButton a, const MouseButton b)
{
    return static_cast<MouseButton>(
        static_cast<SDL_MouseButtonFlags>(a) | static_cast<SDL_MouseButtonFlags>(b));
}

inline const MouseButton operator&(const MouseButton a, const MouseButton b)
{
    return static_cast<MouseButton>(
        static_cast<SDL_MouseButtonFlags>(a) & static_cast<SDL_MouseButtonFlags>(b));
}

inline const MouseButton operator^(const MouseButton a, const MouseButton b)
{
    return static_cast<MouseButton>(
        static_cast<SDL_MouseButtonFlags>(a) ^ static_cast<SDL_MouseButtonFlags>(b));
}

inline const MouseButton operator~(const MouseButton a)
{
    return static_cast<MouseButton>(~static_cast<SDL_MouseButtonFlags>(a));
}

inline MouseButton operator|=(MouseButton& a, const MouseButton b)
{
    return a = static_cast<MouseButton>(
               static_cast<SDL_MouseButtonFlags>(a) | static_cast<SDL_MouseButtonFlags>(b));
}

inline MouseButton operator&=(MouseButton& a, const MouseButton b)
{
    return a = static_cast<MouseButton>(
               static_cast<SDL_MouseButtonFlags>(a) & static_cast<SDL_MouseButtonFlags>(b));
}

inline MouseButton operator^=(MouseButton& a, const MouseButton b)
{
    return a = static_cast<MouseButton>(
               static_cast<SDL_MouseButtonFlags>(a) ^ static_cast<SDL_MouseButtonFlags>(b));
}

inline bool HasFlag(MouseButton button, MouseButton flag)
{
    return std::to_underlying(button) & std::to_underlying(flag);
}

} // namespace Runic
