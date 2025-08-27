#include "Application.hpp"

#include "SDLException.hpp"

#include "SDL3/SDL_init.h"

namespace Runic
{

Application::Application(const AppData& appData, const WindowData& windowData)
{
    Runic::Log::Init(appData.name);

    if (!SDL_Init(SDL_INIT_VIDEO)) throw SDLException("Could not initialize SDL");

    if (!SDL_SetAppMetadata(
            appData.name.c_str(), appData.version.c_str(), appData.identifier.c_str()))
        throw SDLException("Could not set app metadata");

    Window window(windowData);
    std::unique_ptr<GraphicsContext> gc = std::make_unique<GraphicsContext>(window);
    gc->init();

    _renderer = std::make_unique<Renderer>(std::move(gc));
    _renderer->setViewport({windowData.width, windowData.height});

    addSystemEventHandler<WindowCloseEvent>(
        [&](const auto& /*event*/)
        {
            _done = true;
            return true;
        });

    addSystemEventHandler<WindowResizeEvent>(
        [&](const WindowResizeEvent& event)
        {
            _renderer->setViewport({event.width, event.height});
            return true;
        });

    window.show();
}

void Application::onUpdate()
{
    _renderer->beginFrame();

    for (auto& _layer : _layers)
        _layer->onUpdate();

    _renderer->endFrame();
}

} // namespace Runic
