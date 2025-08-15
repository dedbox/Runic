#include "pch.hpp"
#include "Runic/Application.hpp"

#include "glad/gl.h"

namespace Runic
{
Application* Application::s_Instance{nullptr};

static GLenum shaderDataTypeToOpenGLBaseType(const ShaderDataType type)
{
    switch (type) {
        case ShaderDataType::None:
            return GL_NONE;
        case ShaderDataType::Bool:
            return GL_BOOL;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
    }
    RUNIC_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return GL_NONE;
}

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

    constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    };

    _vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));

    // restrict scope to ensure layout object lifetime ownership is transferred to the buffer
    {
        const BufferLayout layout{
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
        };

        _vertexBuffer->setLayout(layout);
    }

    for (const auto&& [index, element] : _vertexBuffer->getLayout().getElements() | std::ranges::views::enumerate) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            element.getComponentCount(),
            shaderDataTypeToOpenGLBaseType(element.type),
            element.normalized ? GL_TRUE : GL_FALSE,
            _vertexBuffer->getLayout().getStride(),
            reinterpret_cast<const void*>(element.offset));
    }

    constexpr uint32_t indices[] = {0, 1, 2};

    _indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

    const std::string vertexSrc = R"EOF(
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

out vec3 v_Position;
out vec4 v_Color;

void main()
{
    v_Position = a_Position;
    v_Color = a_Color;
    gl_Position = vec4(a_Position, 1.0);
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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _shader->bind();
        glBindVertexArray(_vertexArray);
        glDrawElements(GL_TRIANGLES, _indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

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
