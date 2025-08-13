#pragma once

#include "pch.hpp"
#include "Runic/Event/Event.hpp"

namespace Runic
{
struct WindowProps
{
    std::string title{"Runic Engine"};
    unsigned int width{1280};
    unsigned int height{720};
};

class RUNIC_API Window
{
public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() = default;

    virtual void onUpdate() = 0;

    virtual unsigned int getWidth() = 0;

    virtual unsigned int getHeight() = 0;

    virtual void setEventCallback(EventCallbackFn& callback) = 0;

    virtual void setVSync(bool enabled) = 0;

    virtual bool isVSync() const = 0;

    static Window* create(const WindowProps& props = WindowProps());
};
}
