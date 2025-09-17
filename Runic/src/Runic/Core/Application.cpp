#include "Application.hpp"

#include "SDL3/SDL_init.h"

#include "Runic/Core/Event.hpp"
#include "Runic/Core/EventBus.hpp"
#include "Runic/Core/Time.hpp"

namespace Runic
{

Application::Application(const AppData& appData, const WindowData& windowData)
    : _lastFrameTime(Time::Seconds())
{
    Time::Init();
    Log::Init(appData.name);

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

    _context->enableDepthTesting();

    SDL_SetWindowPosition(_window->getNative(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    EventBus::Subscribe<WindowCloseEvent>([&](const auto& /*event*/) { _done = true; });

    EventBus::Subscribe<WindowResizeEvent>([&](const WindowResizeEvent& event) {
        _window->setSize({event.width, event.height});

        std::visit(
            [&](auto&& policy) {
                using T = std::decay_t<decltype(policy)>;
                if constexpr (std::is_same_v<T, AdaptiveResize>)
                    _context->setViewport({event.width, event.height});
            },
            _window->getResizePolicy());
    });

    _window->show();
}

void Application::onUpdate()
{
    const double frameTime = Time::Seconds();
    const double deltaTime = frameTime - _lastFrameTime;
    _lastFrameTime         = frameTime;

    for (auto& _layer : _layers)
        _layer->update(deltaTime);

    const auto [vpSize, vpOffset] = _window->getViewport();

    _renderer->beginFrame();

    std::visit(
        [&](auto&& policy) {
            using T = std::decay_t<decltype(policy)>;
            if constexpr (std::is_same_v<std::decay_t<decltype(policy)>, FixedAspect>)
            {
                _context->setViewport(_window->getSize());
                _context->setClearColor(policy.frameColor);
                _context->clear();

                _context->enableScissor();
                _context->setScissor(vpSize, vpOffset);
                _context->setViewport(vpSize, vpOffset);
            }
        },
        _window->getResizePolicy());

    for (auto& _layer : _layers)
        _layer->render();

    std::visit(
        [&](auto&& policy) {
            using T = std::decay_t<decltype(policy)>;
            if constexpr (std::is_same_v<std::decay_t<decltype(policy)>, FixedAspect>)
                _context->disableScissor();
        },
        _window->getResizePolicy());

    _renderer->endFrame();
}

} // namespace Runic
