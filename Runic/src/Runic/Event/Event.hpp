#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include "spdlog/fmt/fmt.h"

namespace Runic
{
enum class EventType
{
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum class EventCategory : std::uint8_t
{
    None        = 0b00000000,
    Application = 0b00000001,
    Input       = 0b00000010,
    Keyboard    = 0b00000100,
    Mouse       = 0b00001000,
    MouseButton = 0b00010000,
};

inline EventCategory operator|(EventCategory a, EventCategory b)
{
    return static_cast<EventCategory>(static_cast<std::uint8_t>(a) | static_cast<std::uint8_t>(b));
}

inline EventCategory operator&(EventCategory a, EventCategory b)
{
    return static_cast<EventCategory>(static_cast<std::uint8_t>(a) & static_cast<std::uint8_t>(b));
}

inline EventCategory operator^(EventCategory a, EventCategory b)
{
    return static_cast<EventCategory>(static_cast<std::uint8_t>(a) ^ static_cast<std::uint8_t>(b));
}

inline EventCategory operator~(EventCategory c)
{
    return static_cast<EventCategory>(~static_cast<std::uint8_t>(c));
}

inline EventCategory& operator|=(EventCategory& a, EventCategory b)
{
    return a = static_cast<EventCategory>(static_cast<std::uint8_t>(a) | static_cast<std::uint8_t>(b));
}

inline EventCategory& operator&=(EventCategory& a, EventCategory b)
{
    return a = static_cast<EventCategory>(static_cast<std::uint8_t>(a) & static_cast<std::uint8_t>(b));
}

inline EventCategory& operator^=(EventCategory& a, EventCategory b)
{
    return a = static_cast<EventCategory>(static_cast<std::uint8_t>(a) ^ static_cast<std::uint8_t>(b));
}

inline bool hasFlag(EventCategory value, EventCategory flag)
{
    return static_cast<std::uint8_t>(value) & static_cast<std::uint8_t>(flag);
}

class RUNIC_API Event
{
    friend class EventDispatcher;

public:
    virtual ~Event() = default;

    virtual EventType staticType() const = 0;

    virtual EventType type() const { return staticType(); };

    virtual const char* name() const = 0;

    virtual EventCategory category() const = 0;

    virtual std::string toString() const { return name(); }

    bool hasCategory(const EventCategory cat) const { return hasFlag(category(), cat); }

protected:
    bool _handled = false;
};

class EventDispatcher
{
    template<typename T>
    using EventFunction = std::function<bool(T&)>;

public:
    explicit EventDispatcher(Event& event)
        : _event(event) {}

    template<typename T>
    bool dispatch(EventFunction<T> func)
    {
        if (_event.type() == T::staticType()) {
            _event._handled = func(*static_cast<T*>(&_event));
            return true;
        }
        return false;
    }

private:
    Event& _event;
};

auto format_as(const Runic::Event& event)
{
    return fmt::format("{}", event.toString());
}
}
