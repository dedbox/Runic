#include <Runic.hpp>

#include "glm/gtc/matrix_transform.hpp"

class ExampleLayer final : public Runic::Layer
{
public:
    ExampleLayer()
        : Layer("ExampleLayer")
        , _camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        // _vertexArray.reset(Runic::VertexArray::create());
        //
        // constexpr float vertices[]{
        //     -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        //     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        //     0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        // };
        //
        // std::shared_ptr<Runic::VertexBuffer> vertexBuffer;
        // vertexBuffer.reset(Runic::VertexBuffer::create(vertices, sizeof(vertices)));
        //
        // const Runic::BufferLayout layout{
        //     {Runic::ShaderDataType::Float3, "a_Position"},
        //     {Runic::ShaderDataType::Float4, "a_Color"},
        // };
        //
        // vertexBuffer->setLayout(layout);
        // _vertexArray->addVertexBuffer(vertexBuffer);
        //
        // constexpr uint32_t indices[]{0, 1, 2};
        //
        // std::shared_ptr<Runic::IndexBuffer> indexBuffer;
        // indexBuffer.reset(Runic::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
        // _vertexArray->setIndexBuffer(indexBuffer);

        _squareVA.reset(Runic::VertexArray::create());

        constexpr float squareVertices[]{
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
        };

        const auto squareVB{
            std::shared_ptr<Runic::VertexBuffer>(Runic::VertexBuffer::create(squareVertices, sizeof(squareVertices)))
        };
        squareVB->setLayout({
            {Runic::ShaderDataType::Float3, "a_Position"},
        });
        _squareVA->addVertexBuffer(squareVB);

        constexpr uint32_t squareIndices[]{0, 1, 2, 2, 3, 0};

        const auto squareIB{
            std::shared_ptr<Runic::IndexBuffer>(
                Runic::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)))
        };
        _squareVA->setIndexBuffer(squareIB);

        const std::string flatColorShaderVertexSrc = R"EOF(
#version 330 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Position;

void main()
{
    v_Position = a_Position;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
)EOF";

        const std::string flatColorShaderFragmentSrc = R"EOF(
#version 330 core

layout (location = 0) out vec4 color;

in vec3 v_Position;

uniform vec4 u_Color;

void main()
{
    color = u_Color;
}
)EOF";

        _flatColorShader.reset(new Runic::Shader(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
    }

    void onUpdate(Runic::Timestep ts) override
    {
        // RUNIC_TRACE("Delta time: {}s ({} ms)", ts.getSeconds(), ts.getMilliseconds());

        if (Runic::Input::isKeyPressed(Runic::Key::A))
            _cameraPosition.x -= _cameraMoveSpeed * ts;

        if (Runic::Input::isKeyPressed(Runic::Key::D))
            _cameraPosition.x += _cameraMoveSpeed * ts;

        if (Runic::Input::isKeyPressed(Runic::Key::S))
            _cameraPosition.y -= _cameraMoveSpeed * ts;

        if (Runic::Input::isKeyPressed(Runic::Key::W))
            _cameraPosition.y += _cameraMoveSpeed * ts;

        if (Runic::Input::isKeyPressed(Runic::Key::Q))
            _cameraRotation += _cameraRotationSpeed * ts;

        if (Runic::Input::isKeyPressed(Runic::Key::E))
            _cameraRotation -= _cameraRotationSpeed * ts;

        Runic::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Runic::RenderCommand::clear();

        _camera.setPosition(_cameraPosition);
        _camera.setRotation(_cameraRotation);

        Runic::Renderer::beginScene(_camera);

        const glm::mat4 scale{glm::scale(glm::mat4(1.0f), glm::vec3(0.1f))};

        constexpr glm::vec4 redColor(1.0f, 0.0f, 0.0f, 1.0f);
        constexpr glm::vec4 blueColor(0.0f, 0.0f, 1.0f, 1.0f);

        for (int x = 0; x < 20; x++)
            for (int y = 0; y < 20; y++) {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform{glm::translate(glm::mat4(1.0f), pos) * scale};

                if (x % 2)
                    _flatColorShader->uploadUniformFloat4("u_Color", redColor);
                else
                    _flatColorShader->uploadUniformFloat4("u_Color", blueColor);

                Runic::Renderer::submit(_flatColorShader, _squareVA, transform);
            }

        // Runic::Renderer::submit(_flatColorShader, _vertexArray);

        Runic::Renderer::endScene();
    }

private:
    std::shared_ptr<Runic::Shader> _flatColorShader;
    // std::shared_ptr<Runic::VertexArray> _vertexArray;
    std::shared_ptr<Runic::VertexArray> _squareVA;

    Runic::OrthographicCamera _camera;

    glm::vec3 _cameraPosition{0.0f, 0.0f, 0.0f};
    float _cameraMoveSpeed{5.0f};

    float _cameraRotation{0.0f};
    float _cameraRotationSpeed{180.0f};
};

class SandboxApp final : public Runic::Application
{
public:
    SandboxApp()
    {
        pushLayer(new ExampleLayer());
    }

    ~SandboxApp() override = default;
};

Runic::Application* Runic::CreateApplication()
{
    return new SandboxApp();
}
