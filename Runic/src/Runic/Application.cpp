#include "pch.hpp"
#include "Runic/Application.hpp"

#include "GLFW/glfw3.h"

namespace Runic
{
Application* Application::s_Instance{nullptr};

Application::Application()
{
    RUNIC_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    _window = std::unique_ptr<Window>(Window::create());

    Window::EventCallbackFn callback = [&](Event& event) { onEvent(event); };
    _window->setEventCallback(callback);

    _imGuiLayer = new ImGuiLayer();
    pushOverlay(_imGuiLayer);
}

void Application::run()
{
    while (_isRunning) {
        const float time{static_cast<float>(glfwGetTime())};
        const Timestep timestep{time - _lastFrameTime};
        _lastFrameTime = time;

        for (Layer* layer : _layerStack)
            layer->onUpdate(timestep);

        _imGuiLayer->begin();
        for (Layer* layer : _layerStack)
            layer->onImGuiRender();
        _imGuiLayer->end();

        _window->onUpdate();
    }
}

void Application::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<WindowCloseEvent>([&](const WindowCloseEvent& e) { return onWindowClose(e); });

    for (auto it = _layerStack.end(); it != _layerStack.begin();) {
        (*--it)->onEvent(event);
        if (event.handled)
            break;
    }
}

void Application::pushLayer(Layer* layer)
{
    _layerStack.pushLayer(layer);
}

void Application::pushOverlay(Layer* overlay)
{
    _layerStack.pushOverlay(overlay);
}

bool Application::onWindowClose(const WindowCloseEvent& /*event*/)
{
    _isRunning = false;
    return true;
}
}
