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
        _window->onUpdate();
    }
}

void Application::onEvent(Event& event)
{
    RUNIC_CORE_TRACE(event);

    EventDispatcher dispatcher(event);
    dispatcher.dispatch<WindowCloseEvent>([&](const WindowCloseEvent& e) { return onWindowClose(e); });
}

bool Application::onWindowClose(const WindowCloseEvent& event)
{
    _isRunning = false;
    return true;
}
}
