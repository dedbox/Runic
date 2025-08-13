#include "Runic/Application.hpp"

namespace Runic
{
Application::Application()
{
    _window = std::unique_ptr<Window>(Window::create());

    Window::EventCallbackFn callback = [&](Event& event) { onEvent(event); };
    _window->setEventCallback(callback);
}

void Application::run()
{
    while (_isRunning) {
        for (Layer* layer : _layerStack)
            layer->onUpdate();

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

bool Application::onWindowClose(const WindowCloseEvent& event)
{
    _isRunning = false;
    return true;
}
}
