#include <Runic.hpp>

static constexpr auto Bg = Runic::color(0.2F, 0.3F, 0.3F, 1.0F);

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
            // positon         | color            | texture
             0.5F,  0.5F, 0.0F , 1.0F, 0.0F, 0.0F , 1.0F, 1.0F, // top right
             0.5F, -0.5F, 0.0F , 0.0F, 1.0F, 0.0F , 1.0F, 0.0F, // bottom right
            -0.5F, -0.5F, 0.0F , 0.0F, 0.0F, 1.0F , 0.0F, 0.0F, // bottom left
            -0.5F,  0.5F, 0.0F , 1.0F, 1.0F, 0.0F , 0.0F, 1.0F, // top left
        };
        // clang-format on

        const std::vector<Runic::VertexAttribute> layout = {
            {.type = Runic::AttributeType::Float3, .normalize = false}, // a_Position
            {.type = Runic::AttributeType::Float3, .normalize = false}, // a_Color
            {.type = Runic::AttributeType::Float2, .normalize = false}, // a_TexCoord
        };

        // clang-format off
        const std::vector<uint32_t> indices = {
            0, 1, 3,
            1, 2, 3,
        };
        // clang-format on

        _mesh = Runic::Mesh::Create(_context);
        _mesh->addVertices(vertices, layout, Runic::BufferUsage::Static);
        _mesh->setIndices(
            indices,
            Runic::IndexMode::Triangles,
            Runic::IndexType::Int,
            Runic::BufferUsage::Static);

        _mesh->addTexture(Runic::TextureManager::Find(_context, "container.jpg"));
        _mesh->addTexture(
            Runic::TextureManager::Find(
                _context,
                "awesomeface.png",
                {.wrapS     = Runic::TextureWrap::ClampEdge,
                 .wrapT     = Runic::TextureWrap::ClampEdge,
                 .magFilter = Runic::TextureMagFilter::Nearest,
                 .mipmap    = false}));

        _shaderProgram =
            Runic::ShaderManager::Find(_context, "PositionColor3TexCoord", "FlatInterpolateTex2");
        _shaderProgram->bind();
        _shaderProgram->setUniform("u_Texture0", static_cast<int>(0)); // container
        _shaderProgram->setUniform("u_Texture1", static_cast<int>(1)); // face
        _shaderProgram->unbind();
    }

    void update() override
    {
        _context->setClearColor(Bg);
        _context->clear();
        _mesh->draw(*_shaderProgram);
    }

private:
    Runic::GraphicsContext* _context;
    std::shared_ptr<Runic::Texture> _texture;
    std::unique_ptr<Runic::Mesh> _mesh;
    std::shared_ptr<Runic::ShaderProgram> _shaderProgram;
};

class Sandbox : public Runic::Application
{
public:
    Sandbox()
        : Application({.name = "Sandbox"})
    {
        Runic::Log::SetLevel(spdlog::level::info);

        auto context = getGraphicsContext();

        getLayerManager().push_back(std::make_unique<TriangleLayer>(context));
    }
};

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<Sandbox>();
}
