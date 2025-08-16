#include "pch.hpp"
#include "Runic/Application.hpp"

#include "Runic/Renderer/RenderCommand.hpp"
#include "Runic/Renderer/Renderer.hpp"

#include "glm/ext/scalar_constants.hpp"

namespace Runic
{
Application* Application::s_Instance{nullptr};

Application::Application()
    : _camera(-1.6f, 1.6f, -0.9f, 0.9f)
{
    RUNIC_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    _window = std::unique_ptr<Window>(Window::create());

    Window::EventCallbackFn callback = [&](Event& event) { onEvent(event); };
    _window->setEventCallback(callback);

    _imGuiLayer = new ImGuiLayer();
    pushOverlay(_imGuiLayer);

    _vertexArray.reset(VertexArray::create());

    constexpr float vertices[]{
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    };

    std::shared_ptr<VertexBuffer> vertexBuffer;
    vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));

    const BufferLayout layout{
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4, "a_Color"},
    };

    vertexBuffer->setLayout(layout);
    _vertexArray->addVertexBuffer(vertexBuffer);

    constexpr uint32_t indices[]{0, 1, 2};

    std::shared_ptr<IndexBuffer> indexBuffer;
    indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
    _vertexArray->setIndexBuffer(indexBuffer);

    _squareVA.reset(VertexArray::create());

    constexpr float squareVertices[]{
        -0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.75f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.75f, 0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.75f, 0.75f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    };

    const auto squareVB{
        std::shared_ptr<VertexBuffer>(VertexBuffer::create(squareVertices, sizeof(squareVertices)))
    };
    squareVB->setLayout({
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4, "a_Color"},
    });
    _squareVA->addVertexBuffer(squareVB);

    constexpr uint32_t squareIndices[]{0, 1, 2, 2, 3, 0};

    const auto squareIB{
        std::shared_ptr<IndexBuffer>(IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)))
    };
    _squareVA->setIndexBuffer(squareIB);

    const std::string vertexSrc = R"EOF(
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

out vec3 v_Position;
out vec4 v_Color;

uniform mat4 u_ViewProjection;

void main()
{
    v_Position = a_Position;
    v_Color = a_Color;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}
)EOF";

    const std::string fragmentSrc = R"EOF(
#version 330 core

layout (location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;

void main()
{
    color = vec4(v_Position * 0.5 + 0.5, 1.0);
    color = v_Color;
}
)EOF";

    _shader.reset(new Shader(vertexSrc, fragmentSrc));
}

void Application::run()
{
    while (_isRunning) {
        RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        RenderCommand::clear();

        Renderer::beginScene(_camera);

        Renderer::submit(_shader, _squareVA);
        Renderer::submit(_shader, _vertexArray);

        Renderer::endScene();

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
