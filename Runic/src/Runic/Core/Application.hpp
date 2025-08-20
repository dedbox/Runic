#pragma once

#include "Window.hpp"

namespace Runic
{

struct AppData
{
    std::string name{"Runic Application"};
    std::string version{"0"};
    std::string identifier{"net.dedbox.runic"};
};

class Application
{
public:
    explicit Application(const AppData& = AppData(), const WindowData& windowData = WindowData());

    virtual ~Application() = default;

    Application(const Application& app)                  = delete;
    Application(Application&& other)                     = delete;
    Application& operator=(const Application& other)     = delete;
    Application& operator=(Application&& other) noexcept = delete;

    [[nodiscard]] const Window& getWindow() const { return *_window; }

    virtual void onUpdate() {};

private:
    AppData _data;
    std::unique_ptr<Window> _window;
};

extern std::unique_ptr<Application> CreateApplication();

} // namespace Runic
