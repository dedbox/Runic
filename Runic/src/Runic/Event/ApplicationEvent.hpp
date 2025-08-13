#pragma once

#include "pch.hpp"
#include "Runic/Event/Event.hpp"

namespace Runic
{
class WindowResizeEvent final : public Event
{
public:
    WindowResizeEvent(const unsigned int width, const unsigned int height)
        : _width(width), _height(height) {}

    unsigned int width() const { return _width; }
    unsigned int height() const { return _height; }

    std::string toString() const override
    {
        return std::format("WindowResizeEvent: {}, {}", _width, _height);
    }

    static EventType staticType() { return EventType::WindowResize; }

    EventType type() const override { return staticType(); }

    const char* name() const override { return "WindowResize"; }

    EventCategory category() const override { return EventCategory::Application; }

private:
    unsigned int _width, _height;
};

class WindowCloseEvent final : public Event
{
public:
    WindowCloseEvent() = default;

    static EventType staticType() { return EventType::WindowClose; }

    EventType type() const override { return staticType(); }

    const char* name() const override { return "WindowClose"; }

    EventCategory category() const override { return EventCategory::Application; }
};

class AppTickEvent final : public Event
{
public:
    AppTickEvent() = default;

    static EventType staticType() { return EventType::AppTick; }

    EventType type() const override { return staticType(); }

    const char* name() const override { return "AppTick"; }

    EventCategory category() const override { return EventCategory::Application; }
};

class AppUpdateEvent final : public Event
{
public:
    AppUpdateEvent() = default;

    static EventType staticType() { return EventType::AppUpdate; }

    EventType type() const override { return staticType(); }

    const char* name() const override { return "AppUpdate"; }

    EventCategory category() const override { return EventCategory::Application; }
};

class AppRenderEvent final : public Event
{
public:
    AppRenderEvent() = default;

    static EventType staticType() { return EventType::AppRender; }

    EventType type() const override { return staticType(); }

    const char* name() const override { return "AppRender"; }

    EventCategory category() const override { return EventCategory::Application; }
};
}
