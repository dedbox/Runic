#pragma once

#include "Runic/Core/Event.hpp"
#include "Runic/Core/LayerManager.hpp"
#include "Runic/Core/Window.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/Renderer.hpp"

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

    Window* getWindow() { return _window.get(); }
    Renderer* getRenderer() { return _renderer.get(); }
    GraphicsContext* getGraphicsContext() { return _context.get(); }
    LayerManager& getLayerManager() { return _layers; }

    virtual void onUpdate();

    template <typename EventType>
    void dispatchEvent(const EventType& event)
    {
        for (auto& _layer : std::ranges::reverse_view(_layers))
            if (_layer->handleEvent(event))
                return;
        handleSystemEvent(event);
    }

    bool isDone() const { return _done; }

protected:
    template <typename EventType>
    void addSystemEventHandler(std::function<bool(const EventType&)> handler)
    {
        _systemDispatcher.addHandler<EventType>(handler);
    }

    template <typename EventType>
    bool handleSystemEvent(const EventType& event)
    {
        return _systemDispatcher.dispatch(event);
    }

private:
    // components
    std::unique_ptr<Window> _window;
    std::unique_ptr<Renderer> _renderer;
    std::unique_ptr<GraphicsContext> _context;
    LayerManager _layers;

    // events
    EventDispatcher _systemDispatcher;
    bool _done = false;

    // timing
    double _lastFrameTime;
};

extern std::unique_ptr<Application> CreateApplication();

} // namespace Runic
