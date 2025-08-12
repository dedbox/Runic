#pragma once

#include <format>

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

    EventType staticType() const override { return EventType::WindowResize; }

    const char* name() const override { return "WindowResize"; }

    EventCategory category() const override { return EventCategory::Application; }

private:
    unsigned int _width, _height;
};

class WindowCloseEvent final : public Event
{
public:
    WindowCloseEvent() = default;

    EventType staticType() const override { return EventType::WindowClose; }

    const char* name() const override { return "WindowClose"; }

    EventCategory category() const override { return EventCategory::Application; }
};

class AppTickEvent final : public Event
{
public:
    AppTickEvent() = default;

    EventType staticType() const override { return EventType::AppTick; }

    const char* name() const override { return "AppTick"; }

    EventCategory category() const override { return EventCategory::Application; }
};

class AppUpdateEvent final : public Event
{
public:
    AppUpdateEvent() = default;

    EventType staticType() const override { return EventType::AppUpdate; }

    const char* name() const override { return "AppUpdate"; }

    EventCategory category() const override { return EventCategory::Application; }
};

class AppRenderEvent final : public Event
{
public:
    AppRenderEvent() = default;

    EventType staticType() const override { return EventType::AppRender; }

    const char* name() const override { return "AppRender"; }

    EventCategory category() const override { return EventCategory::Application; }
};
}
