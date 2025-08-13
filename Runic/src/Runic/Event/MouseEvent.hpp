#pragma once

#include "pch.hpp"
#include "Runic/Event/Event.hpp"

namespace Runic
{
class RUNIC_API MouseMovedEvent final : public Event
{
public:
    MouseMovedEvent(const float x, const float y)
        : _mouseX(x), _mouseY(y) {}

    float x() const { return _mouseX; }
    float y() const { return _mouseY; }

    std::string toString() const override
    {
        return std::format("MouseMovedEvent: {}, {}", _mouseX, _mouseY);
    }

    static EventType staticType() { return EventType::MouseMoved; }

    EventType type() const override { return staticType(); }

    const char* name() const override { return "MouseMoved"; }

    EventCategory category() const override { return EventCategory::Mouse | EventCategory::Input; }

private:
    float _mouseX, _mouseY;
};

class RUNIC_API MouseScrolledEvent final : public Event
{
public:
    MouseScrolledEvent(const float xOffset, const float yOffset)
        : _xOffset(xOffset), _yOffset(yOffset) {}

    float xOffset() const { return _xOffset; }
    float yOffset() const { return _yOffset; }

    std::string toString() const override
    {
        return std::format("MouseScrolledEvent: {}, {}", _xOffset, _yOffset);
    }

    static EventType staticType() { return EventType::MouseScrolled; }

    EventType type() const override { return staticType(); }

    const char* name() const override { return "MouseScrolled"; }

    EventCategory category() const override { return EventCategory::Mouse | EventCategory::Input; }

private:
    float _xOffset, _yOffset;
};

class RUNIC_API MouseButtonEvent : public Event
{
public:
    int button() const { return _button; }

    EventCategory category() const override { return EventCategory::Mouse | EventCategory::Input; }

protected:
    explicit MouseButtonEvent(const int button)
        : _button(button) {}

    int _button;
};

class RUNIC_API MouseButtonPressedEvent final : public MouseButtonEvent
{
public:
    explicit MouseButtonPressedEvent(const int button)
        : MouseButtonEvent(button) {}

    std::string toString() const override
    {
        return std::format("MouseButtonPressedEvent: {}", _button);
    }

    static EventType staticType() { return EventType::MouseButtonPressed; }

    EventType type() const override { return staticType(); }

    const char* name() const override { return "MouseButtonPressed"; }
};

class RUNIC_API MouseButtonReleasedEvent final : public MouseButtonEvent
{
public:
    explicit MouseButtonReleasedEvent(const int button)
        : MouseButtonEvent(button) {}

    std::string toString() const override
    {
        return std::format("MouseButtonReleasedEvent: {}", _button);
    }

    static EventType staticType() { return EventType::MouseButtonReleased; }

    EventType type() const override { return staticType(); }

    const char* name() const override { return "MouseButtonReleased"; }
};
}
