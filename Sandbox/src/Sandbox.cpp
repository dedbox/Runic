#include <Runic.hpp>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

static constexpr auto Bg = Runic::color(0.2F, 0.3F, 0.3F, 1.0F);

static constexpr auto Taupe  = Runic::Color::hex(0x463F3AFF);
static constexpr auto Gray   = Runic::Color::hex(0x8A817CFF);
static constexpr auto Silver = Runic::Color::hex(0xBCB8B1FF);
static constexpr auto Ivory  = Runic::Color::hex(0xF4F3EEFF);
static constexpr auto Melon  = Runic::Color::hex(0xE0AFA0FF);

// TriangleLayer -----------------------------------------------------------------------------------

class TriangleLayer : public Runic::Layer
{
public:
    explicit TriangleLayer(Runic::GraphicsContext* context)
        : _context(context)
    {
        // clang-format off
        const std::vector<float> vertices = {
            // position        |  texcoord
            -0.5F, -0.5F, -0.5F,  0.0F, 0.0F,
             0.5F, -0.5F, -0.5F,  1.0F, 0.0F,
             0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
             0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
            -0.5F,  0.5F, -0.5F,  0.0F, 1.0F,
            -0.5F, -0.5F, -0.5F,  0.0F, 0.0F,

            -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
             0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
             0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
             0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
            -0.5F,  0.5F,  0.5F,  0.0F, 1.0F,
            -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,

            -0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
            -0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
            -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
            -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
            -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
            -0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

             0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
             0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
             0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
             0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
             0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
             0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

            -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
             0.5F, -0.5F, -0.5F,  1.0F, 1.0F,
             0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
             0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
            -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
            -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,

            -0.5F,  0.5F, -0.5F,  0.0F, 1.0F,
             0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
             0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
             0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
            -0.5F,  0.5F,  0.5F,  0.0F, 0.0F,
            -0.5F,  0.5F, -0.5F,  0.0F, 1.0F,
        };
        // clang-format on

        const std::vector<Runic::VertexAttribute> layout = {
            {.type = Runic::AttributeType::Float3, .normalize = false}, // a_Position
            {.type = Runic::AttributeType::Float2, .normalize = false}, // a_TexCoord
        };

        _mesh = Runic::Mesh::Create(_context, Runic::DrawMode::Triangles);
        _mesh->addVertices(vertices, layout, Runic::BufferUsage::Static);

        _mesh->addTexture(Runic::TextureManager::Find(_context, "container.jpg"));
        _mesh->addTexture(
            Runic::TextureManager::Find(
                _context,
                "awesomeface.png",
                {.wrapS     = Runic::TextureWrap::ClampEdge,
                 .wrapT     = Runic::TextureWrap::ClampEdge,
                 .magFilter = Runic::TextureMagFilter::Nearest,
                 .mipmap    = false}));

        _shaderProgram = Runic::ShaderManager::Find(_context, "PosTexMVP", "Tex2Mix");

        _view       = glm::translate(glm::mat4(1.0F), glm::vec3(0.0F, 0.0F, -3.0F));
        _projection = glm::perspective(glm::radians(45.0F), 1280.0F / 720.0F, 0.1F, 100.0F);

        _shaderProgram->bind();
        _shaderProgram->setUniform("u_Texture0", static_cast<int>(0)); // container
        _shaderProgram->setUniform("u_Texture1", static_cast<int>(1)); // face
        _shaderProgram->setUniform("u_View", _view);
        _shaderProgram->setUniform("u_Projection", _projection);
        _shaderProgram->unbind();
    }

    void render() override
    {
        _context->setClearColor(Bg);
        _context->clear();

        for (auto&& [i, pos] : _cubePositions | std::ranges::views::enumerate)
        {
            auto model  = glm::mat4(1.0F);
            model       = glm::translate(model, pos);
            float angle = 20.0F * static_cast<float>(i);
            model       = glm::rotate(model, glm::radians(angle), glm::vec3(1.0F, 0.3F, 0.5F));
            model       = glm::rotate(
                model,
                static_cast<float>(Runic::Time::Seconds()) * glm::radians(50.0F),
                glm::vec3(0.5F, 1.0F, 0.0F));
            _shaderProgram->bind();
            _shaderProgram->setUniform("u_Model", model);
            _shaderProgram->unbind();
            _mesh->draw(*_shaderProgram);
        }
    }

private:
    Runic::GraphicsContext* _context;
    std::shared_ptr<Runic::Texture> _texture;
    std::unique_ptr<Runic::Mesh> _mesh;
    std::shared_ptr<Runic::ShaderProgram> _shaderProgram;
    glm::mat4 _view{1.0F}, _projection{1.0F};
    // clang-format off
    std::vector<glm::vec3> _cubePositions = {
        glm::vec3( 0.0F,  0.0F,   0.0F),
        glm::vec3( 2.0F,  5.0F, -15.0F),
        glm::vec3(-1.5F, -2.2F,  -2.5F),
        glm::vec3(-3.8F, -2.0F, -12.3F),
        glm::vec3( 2.4F, -0.4F,  -3.5F),
        glm::vec3(-1.7F,  3.0F,  -7.5F),
        glm::vec3( 1.3F, -2.0F,  -2.5F),
        glm::vec3( 1.5F,  2.0F,  -2.5F),
        glm::vec3( 1.5F,  0.2F,  -1.5F),
        glm::vec3(-1.3F,  1.0F,  -1.5F),
    };
    // clang-format on
};

// Sandbox -----------------------------------------------------------------------------------------

class Sandbox : public Runic::Application
{
public:
    Sandbox()
        : Application({.name = "Sandbox"})
    {
        Runic::Log::SetLevel(spdlog::level::info);

        auto window  = getWindow();
        auto context = getGraphicsContext();

        auto& layers = getLayerManager();
        layers.push_back(std::make_unique<TriangleLayer>(context));
    }
};

// Application -------------------------------------------------------------------------------------

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<Sandbox>();
}
