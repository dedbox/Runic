#pragma once

#include "Event.hpp"
#include "LayerManager.hpp"
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

    Renderer& getRenderer() { return *_renderer; }
    GraphicsContext& getGraphicsContext() { return _renderer->getGraphicsContext(); }
    Window& getWindow() { return _renderer->getGraphicsContext().getWindow(); }
    LayerManager& getLayerManager() { return _layers; }

    virtual void onUpdate();

    template <typename EventType>
    void dispatchEvent(const EventType& event)
    {
        for (auto& _layer : std::ranges::reverse_view(_layers))
            if (_layer->handleEvent(event)) return;
        handleSystemEvent(event);
    }

    [[nodiscard]] bool isDone() const { return _done; }

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
    std::unique_ptr<Renderer> _renderer;
    EventDispatcher _systemDispatcher;
    LayerManager _layers;
    bool _done{false};
};

extern std::unique_ptr<Application> CreateApplication();

} // namespace Runic
