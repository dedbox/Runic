#include "Runic/Application.hpp"

namespace Runic
{
Application::Application()
{
    _window = std::unique_ptr<Window>(Window::create());
}

void Application::run()
{
    while (_isRunning) {
        _window->onUpdate();
    }
}
}
