#pragma once
#include "GLFW/glfw3.h"
#include "Runic/Window.hpp"

namespace Runic
{
class LinuxWindow : public Window
{
public:
    explicit LinuxWindow(const WindowProps& props);

    ~LinuxWindow() override;

    void onUpdate() override;

    unsigned int getWidth() override { return _data.width; }
    unsigned int getHeight() override { return _data.height; }

    void setEventCallback(EventCallbackFn& callback) override { _data.eventCallback = callback; }

    void setVSync(bool enabled) override;

    bool isVSync() const override;

private:
    virtual void init(const WindowProps& props);

    virtual void shutdown();

    GLFWwindow* _window;

    struct WindowData
    {
        std::string title;
        unsigned int width, height;
        bool vsync;
        EventCallbackFn eventCallback;
    };

    WindowData _data;
};
}
