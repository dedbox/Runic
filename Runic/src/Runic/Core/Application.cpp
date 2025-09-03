#include "Application.hpp"

#include "SDL3/SDL_init.h"

namespace Runic
{

Application::Application(const AppData& appData, const WindowData& windowData)
{
    Runic::Log::Init(appData.name);

    const std::string cwd = std::filesystem::current_path();
    Core::Info("Current working directory is {}", cwd);

    if (!SDL_Init(SDL_INIT_VIDEO))
        throw SDLException("Could not initialize SDL");

    if (!SDL_SetAppMetadata(
            appData.name.c_str(), appData.version.c_str(), appData.identifier.c_str()))
        throw SDLException("Could not set app metadata");

    _window = std::make_unique<Window>(windowData);

    _context = std::make_unique<GraphicsContext>(_window->getNative());
    _context->init();

    _renderer = std::make_unique<Renderer>(_window->getNative());
    _context->setViewport({windowData.width, windowData.height});

    SDL_SetWindowPosition(_window->getNative(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    addSystemEventHandler<WindowCloseEvent>([&](const auto& /*event*/) {
        _done = true;
        return true;
    });

    addSystemEventHandler<WindowResizeEvent>([&](const WindowResizeEvent& event) {
        _context->setViewport({event.width, event.height});
        return true;
    });

    _window->show();
}

void Application::onUpdate()
{
    _renderer->beginFrame();

    for (auto& _layer : _layers)
    {
        _layer->update();
        _layer->render();
    }

    _renderer->endFrame();
}

} // namespace Runic
