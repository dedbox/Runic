#include "Runic/Renderer/GraphicsContext.hpp"
#include <Runic.hpp>
#include <spdlog/common.h>

static constexpr auto Taupe  = Runic::Color::hex(0x463F3AFF);
static constexpr auto Gray   = Runic::Color::hex(0x8A817CFF);
static constexpr auto Silver = Runic::Color::hex(0xBCB8B1FF);
static constexpr auto Ivory  = Runic::Color::hex(0xF4F3EEFF);
static constexpr auto Melon  = Runic::Color::hex(0xE0AFA0FF);

class TriangleLayer : public Runic::Layer
{
public:
    explicit TriangleLayer(Runic::GraphicsContext* context)
        : _context(context)
    {
        // clang-format off
        const std::vector<float> vertices = {
            //    positon      |     color
            -0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 0.0F,
             0.5F, -0.5F, 0.0F, 0.0F, 1.0F, 0.0F,
             0.0F,  0.5F, 0.0F, 0.0F, 0.0F, 1.0F,
        };
        // clang-format on

        const std::vector<Runic::VertexAttribute> layout = {
            {.type = Runic::AttributeType::Float3, .normalize = false},
            {.type = Runic::AttributeType::Float3, .normalize = false}};

        const std::vector<uint32_t> indices = {0, 1, 2};

        _mesh = std::make_unique<Runic::Mesh>(_context);
        _mesh->addVertices(vertices, layout, Runic::BufferUsage::Static);
        _mesh->setIndices(
            indices,
            Runic::IndexMode::Triangles,
            Runic::IndexType::Int,
            Runic::BufferUsage::Static);

        _shaderProgram = Runic::ShaderManager::getShader("PositionColor3-FlatInterpolate");
    }

    void update() override
    {
        _context->setClearColor(Taupe);
        _context->clear();

        _mesh->draw(*_shaderProgram);
    }

private:
    Runic::GraphicsContext* _context;
    std::unique_ptr<Runic::Mesh> _mesh;
    std::shared_ptr<Runic::ShaderProgram> _shaderProgram;
};

class Sandbox : public Runic::Application
{
public:
    Sandbox()
        : Application({.name = "Sandbox"})
    {
        Runic::ShaderManager::LoadShader(
            getGraphicsContext(),
            "Position-FlatUniform",
            "shaders/Position.vert",
            "shaders/FlatUniform.frag");
        Runic::ShaderManager::LoadShader(
            getGraphicsContext(),
            "PositionColor3-FlatInterpolate",
            "shaders/PositionColor3.vert",
            "shaders/FlatInterpolate.frag");
        getLayerManager().push_back(std::make_unique<TriangleLayer>(getGraphicsContext()));
    }
};

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<Sandbox>();
}
