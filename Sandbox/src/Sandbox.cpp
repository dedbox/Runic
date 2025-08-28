#include <Runic.hpp>

static constexpr auto Taupe  = Runic::Color::hex(0x463F3AFF);
static constexpr auto Gray   = Runic::Color::hex(0x8A817CFF);
static constexpr auto Silver = Runic::Color::hex(0xBCB8B1FF);
static constexpr auto Ivory  = Runic::Color::hex(0xF4F3EEFF);
static constexpr auto Melon  = Runic::Color::hex(0xE0AFA0FF);

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

        const std::vector<Runic::VertexAttribute> layout = {
            {.type = Runic::AttributeType::Float3, .normalize = false}};

        const std::vector<uint32_t> indices = {0, 1, 2};

        _mesh = std::make_unique<Runic::Mesh>(&_renderer);
        _mesh->addVertices(vertices, layout, Runic::BufferUsage::Static);
        _mesh->setIndices(indices, Runic::IndexMode::Triangles, Runic::BufferUsage::Static);

        _shaderProgram = Runic::ShaderManager::getShader("Position-FlatUniform");
        _shaderProgram->use();
        _shaderProgram->setUniform("u_Color", Melon);
    }

    void onUpdate() override
    {
        _renderer.setClearColor(Taupe);
        _renderer.clear();
        // _renderer.getGraphicsContext().setPolygonMode(Runic::PolygonMode::Line);
        _mesh->draw(*_shaderProgram);
    }

private:
    Runic::Renderer& _renderer;

    std::unique_ptr<Runic::Mesh> _mesh;
    std::shared_ptr<Runic::ShaderProgram> _shaderProgram;
};

class SandboxApplication : public Runic::Application
{
public:
    SandboxApplication()
        : Application({.name = "Sandbox"})
        , _renderer(getRenderer())
    {
        Runic::ShaderManager::LoadShader(
            getRenderer(), "Position-FlatUniform", "shaders/Position.vert",
            "shaders/FlatUniform.frag");
        getLayerManager().push_back(std::make_unique<TriangleLayer>(getRenderer()));
    }

private:
    Runic::Renderer& _renderer;
};

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<SandboxApplication>();
}
