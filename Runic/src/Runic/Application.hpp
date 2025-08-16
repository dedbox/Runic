#pragma once

#include "pch.hpp"
#include "Runic/Event/ApplicationEvent.hpp"
#include "Runic/ImGui/ImGuiLayer.hpp"
#include "Runic/Layer.hpp"
#include "Runic/LayerStack.hpp"
#include "Runic/Window.hpp"

#include "Runic/Renderer/Camera.hpp"
#include "Runic/Renderer/Shader.hpp"
#include "Runic/Renderer/VertexArray.hpp"

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

    std::shared_ptr<Shader> _shader;
    std::shared_ptr<VertexArray> _vertexArray;
    std::shared_ptr<VertexArray> _squareVA;

    OrthographicCamera _camera;

    bool onWindowClose(const WindowCloseEvent& event);
};

// Client must define this
extern Application* CreateApplication();
}
