#include "pch.hpp"
#include "Runic/Application.hpp"

#include "glad/gl.h"

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
    pushLayer(_imGuiLayer);

    glGenVertexArrays(1, &_vertexArray);
    glBindVertexArray(_vertexArray);

    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

    constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glGenBuffers(1, &_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

    constexpr unsigned int indices[] = {0, 1, 2};

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Application::run()
{
    while (_isRunning) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(_vertexArray);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        for (Layer* layer : _layerStack)
            layer->onUpdate();

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
