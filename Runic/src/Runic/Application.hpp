#pragma once

#include "pch.hpp"
#include "Runic/Event/ApplicationEvent.hpp"
#include "Runic/ImGui/ImGuiLayer.hpp"
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

    static Application& get()
    {
        RUNIC_CORE_ASSERT(s_Instance, "Application has not been instantiated!")
        assert(s_Instance != nullptr);
        return *s_Instance;
    }

    Window& getWindow() const { return *_window; }

private:
    static Application* s_Instance;

    std::unique_ptr<Window> _window;
    ImGuiLayer* _imGuiLayer;

    bool _isRunning = true;
    LayerStack _layerStack;

    bool onWindowClose(const WindowCloseEvent& event);
};

// Client must define this
extern Application* CreateApplication();
}
