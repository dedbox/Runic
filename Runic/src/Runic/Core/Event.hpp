#pragma once

#include "Runic/Core/Input/Key.hpp"
#include "Runic/Core/Input/Mouse.hpp"

namespace Runic
{

// Window Events -----------------------------------------------------------------------------------

struct WindowCloseEvent
{
};

struct WindowFocusEvent
{
};

struct WindowResizeEvent
{
    uint32_t width, height;
};

struct WindowUnfocusEvent
{
};

// Keyboard Events ---------------------------------------------------------------------------------

struct KeyPressEvent
{
    Key key;
    bool repeat;
};

struct KeyReleaseEvent
{
    Key key;
};

// Mouse Events ------------------------------------------------------------------------------------

struct MouseButtonPressEvent
{
    MouseButton button;
    float x, y;
};

struct MouseButtonReleaseEvent
{
    MouseButton button;
    float x, y;
};

struct MouseMoveEvent
{
    float xOffset, yOffset;
};

struct MouseScrollEvent
{
    float horiz, vert;
};

// -------------------------------------------------------------------------------------------------

// clang-format off
using EventVariant = std::variant<
    WindowCloseEvent, WindowFocusEvent, WindowResizeEvent, WindowUnfocusEvent,
    KeyPressEvent, KeyReleaseEvent,
    MouseButtonPressEvent, MouseButtonReleaseEvent, MouseMoveEvent, MouseScrollEvent>;
// clang-format on

} // namespace Runic
