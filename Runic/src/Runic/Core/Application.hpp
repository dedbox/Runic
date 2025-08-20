#pragma once

#include "Event.hpp"
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

    template <typename EventType>
    void addEventHandler(std::function<bool(const EventType&)> handler)
    {
        _userDispatcher.addHandler<EventType>(handler);
    }

    template <typename EventType>
    void handleEvent(const EventType& event)
    {
        _userDispatcher.dispatch(event) || _systemDispatcher.dispatch(event);
    }

    [[nodiscard]] bool isDone() const { return _done; }

private:
    AppData _data;
    std::unique_ptr<Window> _window;
    EventDispatcher _userDispatcher, _systemDispatcher;
    bool _done{false};

    template <typename EventType>
    void addSystemEventHandler(std::function<bool(const EventType&)> handler)
    {
        _systemDispatcher.addHandler<EventType>(handler);
    }

    template <typename EventType>
    void handleSystemEvent(const EventType& event)
    {
        _systemDispatcher.dispatch(event);
    }
};

extern std::unique_ptr<Application> CreateApplication();

} // namespace Runic
