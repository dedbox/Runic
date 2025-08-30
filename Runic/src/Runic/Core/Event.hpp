#pragma once

#include "Input/Key.hpp"
#include "Input/Mouse.hpp"

namespace Runic
{

// Window Events ---------------------------------------------------------------

struct WindowCloseEvent
{
};

struct WindowFocusEvent
{
};

struct WindowResizeEvent
{
    int width, height;
};

struct WindowUnfocusEvent
{
};

//  Key Events -----------------------------------------------------------------

struct KeyPressEvent
{
    Key key;
    bool repeat;
};

struct KeyReleaseEvent
{
    Key key;
};

// Mouse Events ----------------------------------------------------------------

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

// -----------------------------------------------------------------------------

class EventDispatcher
{
public:
    using TypeErasedHandler = std::function<bool(const void*)>;

    EventDispatcher()  = default;
    ~EventDispatcher() = default;

    // allow moving
    EventDispatcher(EventDispatcher&&) noexcept            = default;
    EventDispatcher& operator=(EventDispatcher&&) noexcept = default;

    // prevent copying
    EventDispatcher(const EventDispatcher&)            = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;

    template <typename EventType>
    void addHandler(std::function<bool(const EventType&)> handler)
    {
        _handlers[typeid(EventType)].push_back(
            [handler](const void* event)
            { return handler(*static_cast<const EventType*>(event)); });
    }

    template <typename EventType>
    bool dispatch(const EventType& event)
    {
        auto it{_handlers.find(typeid(EventType))};
        if (it != _handlers.end())
            for (const auto& handler : it->second)
                if (handler(&event)) return true;
        return false;
    }

private:
    std::map<std::type_index, std::vector<TypeErasedHandler>> _handlers;
};

} // namespace Runic
