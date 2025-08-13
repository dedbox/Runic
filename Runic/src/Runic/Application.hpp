#pragma once

#include "pch.hpp"
#include "Runic/Event/ApplicationEvent.hpp"
#include "Runic/Layer.hpp"
#include "Runic/LayerStack.hpp"
#include "Runic/Window.hpp"

namespace Runic
{
class RUNIC_API Application
{
public:
    Application();

    virtual ~Application() = default;

    void run();

    void onEvent(Event& event);

    void pushLayer(Layer* layer);

    void pushOverlay(Layer* overlay);

private:
    std::unique_ptr<Window> _window;
    bool _isRunning = true;
    LayerStack _layerStack;

    bool onWindowClose(const WindowCloseEvent& event);
};

// Client must define this
extern Application* CreateApplication();
}
