#include <Runic.hpp>

#include "Runic/Renderer/Buffer.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/VertexArray.hpp"

static constexpr auto Taupe  = Runic::Color::hex(0x463F3AFF);
static constexpr auto Gray   = Runic::Color::hex(0x8A817CFF);
static constexpr auto Silver = Runic::Color::hex(0xBCB8B1FF);
static constexpr auto Ivory  = Runic::Color::hex(0xF4F3EEFF);
static constexpr auto Melon  = Runic::Color::hex(0xE0AFA0FF);

namespace
{

constexpr std::string FlatShaderVertexSrc()
{
    return R"EOF(
#version 330 core

layout (location = 0) in vec3 a_Position;

void main()
{
    gl_Position = vec4(a_Position, 1.0);
}
)EOF";
}

constexpr std::string FlatShaderFragmentSrc()
{
    return R"EOF(
#version 330 core

layout (location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
    color = u_Color;
}
)EOF";
}

} // namespace

class TriangleLayer : public Runic::Layer
{
public:
    explicit TriangleLayer(Runic::Renderer& renderer)
        : _renderer(renderer)
    {
        // clang-format off
        const std::vector<float> vertices = {
            -0.5F, -0.5F, 0.0F,
             0.5F, -0.5F, 0.0F,
             0.0F,  0.5F, 0.0F,
        };
        // clang-format on

        const std::vector<Runic::LayoutElement> layout = {
            {.type = Runic::ElementType::Float3, .normalize = false}};

        const std::vector<uint32_t> indices = {0, 1, 2};

        _vertexArray = _renderer.createVertexArray();

        _vertexBuffer = _renderer.createVertexBuffer(
            vertices.data(), vertices.size() * sizeof(float), Runic::BufferUsage::Static);

        _vertexArray->addVertexBuffer(std::move(_vertexBuffer), layout);

        _indexBuffer = _renderer.createIndexBuffer(
            indices.data(), indices.size(), Runic::IndexType::Int, Runic::IndexMode::Triangles,
            Runic::BufferUsage::Static);

        _vertexArray->setIndexBuffer(std::move(_indexBuffer));

        _vertexShader = _renderer.createShader(FlatShaderVertexSrc(), Runic::ShaderType::Vertex);
        _fragmentShader =
            _renderer.createShader(FlatShaderFragmentSrc(), Runic::ShaderType::Fragment);

        _shaderProgram = _renderer.createShaderProgram(*_vertexShader, *_fragmentShader);
        _shaderProgram->use();
        _shaderProgram->setUniformFloat4("u_Color", Melon);
    }

    void onUpdate() override
    {
        _renderer.setClearColor(Taupe);
        _renderer.clear();

        _shaderProgram->use();
        _vertexArray->bind();
        _vertexArray->draw();
    }

private:
    Runic::Renderer& _renderer;

    std::unique_ptr<Runic::VertexArray> _vertexArray;
    std::unique_ptr<Runic::VertexBuffer> _vertexBuffer;
    std::unique_ptr<Runic::IndexBuffer> _indexBuffer;
    std::unique_ptr<Runic::Shader> _vertexShader;
    std::unique_ptr<Runic::Shader> _fragmentShader;
    std::unique_ptr<Runic::ShaderProgram> _shaderProgram;
};

class SandboxApplication : public Runic::Application
{
public:
    SandboxApplication()
        : Application({.name = "Sandbox"})
        , _renderer(getRenderer())
    {
        getLayerManager().push_back(std::make_unique<TriangleLayer>(getRenderer()));
    }

private:
    Runic::Renderer& _renderer;
};

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<SandboxApplication>();
}
