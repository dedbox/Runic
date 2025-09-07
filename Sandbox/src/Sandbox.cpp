#include <Runic.hpp>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

#include "Runic/Renderer/Attribute.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/VertexData.hpp"

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
    using Pos3 = Runic::Attribute<float, 3>;
    using Tex2 = Runic::Attribute<float, 2>;

    using Pos3Tex2 = Runic::Layout<Pos3, Tex2>;

    TriangleLayer(Runic::Window* window, Runic::GraphicsContext* context)
        : _window(window)
        , _context(context)
    {

        // clang-format off
        const Runic::VertexData<Pos3Tex2> vertices({
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
            });
        // clang-format on

        const std::vector<Runic::VertexAttribute> layout = {
            {.type = Runic::AttributeType::Float3, .normalize = false}, // a_Position
            {.type = Runic::AttributeType::Float2, .normalize = false}, // a_TexCoord
        };

        _mesh = Runic::Mesh<Pos3Tex2>::Create(_context, Runic::DrawMode::Triangles);
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

        _shaderProgram->bind();
        _shaderProgram->setUniform("u_Texture0", static_cast<int>(0)); // container
        _shaderProgram->setUniform("u_Texture1", static_cast<int>(1)); // face

        std::visit(
            [&](auto&& policy) {
                using T = std::decay_t<decltype(policy)>;
                if constexpr (std::is_same_v<T, Runic::FixedAspect>)
                {
                    _projection =
                        glm::perspective(glm::radians(45.0F), policy.aspectRatio, 0.1F, 100.0F);
                    _shaderProgram->setUniform("u_Projection", _projection);
                }
            },
            _window->getResizePolicy());

        _shaderProgram->unbind();
    }

    void attach() override
    {
        // _window->captureMouse();

        std::visit(
            [&](auto&& policy) {
                using T = std::decay_t<decltype(policy)>;
                if constexpr (std::is_same_v<T, Runic::AdaptiveResize>)
                {
                    const auto size = _window->getSize();
                    _projection     = glm::perspective(
                        glm::radians(45.0F),
                        static_cast<float>(size.x) / static_cast<float>(size.y),
                        0.1F,
                        100.0F);

                    addEventHandler<Runic::WindowResizeEvent>(
                        [&](const Runic::WindowResizeEvent& event) {
                            _projection = glm::perspective(
                                glm::radians(45.0F),
                                static_cast<float>(event.width) / static_cast<float>(event.height),
                                0.1F,
                                100.0F);

                            return false;
                        });
                }
            },
            _window->getResizePolicy());

        addEventHandler<Runic::KeyPressEvent>([&](const auto& event) {
            if (event.key == Runic::Key::Escape)
            {
                if (_window->isMouseCaptured())
                    _window->releaseMouse();
                else
                    _window->captureMouse();
                return true;
            }
            return false;
        });
    }

    void detach() override { _window->releaseMouse(); }

    void update(double deltaTime) override
    {
        const float cameraSpeed = 2.5F * static_cast<float>(deltaTime);

        if (Runic::Input::IsKeyPressed(Runic::Key::W))
            _cameraPos += cameraSpeed * _cameraFront;

        if (Runic::Input::IsKeyPressed(Runic::Key::S))
            _cameraPos -= cameraSpeed * _cameraFront;

        if (Runic::Input::IsKeyPressed(Runic::Key::A))
            _cameraPos -= glm::normalize(glm::cross(_cameraFront, _cameraUp)) * cameraSpeed;

        if (Runic::Input::IsKeyPressed(Runic::Key::D))
            _cameraPos += glm::normalize(glm::cross(_cameraFront, _cameraUp)) * cameraSpeed;
    }

    void render() override
    {
        _context->setClearColor(Bg);
        _context->clear();

        for (auto&& [i, pos] : _cubePositions | std::ranges::views::enumerate)
        {
            auto model = glm::translate(glm::mat4(1.0F), pos);
            model      = glm::rotate(
                model, glm::radians(20.0F * static_cast<float>(i)), glm::vec3(1.0F, 0.3F, 0.5F));
            model = glm::rotate(
                model,
                static_cast<float>(Runic::Time::Seconds()) * glm::radians(50.0F),
                glm::vec3(0.5F, 1.0F, 0.0F));
            _shaderProgram->bind();
            _shaderProgram->setUniform("u_Model", model);
            _shaderProgram->unbind();
            _mesh->draw(*_shaderProgram);
        }

        glm::mat4 view = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
        _shaderProgram->bind();
        _shaderProgram->setUniform("u_View", view);

        std::visit(
            [&](auto&& policy) {
                using T = std::decay_t<decltype(policy)>;
                if constexpr (std::is_same_v<T, Runic::AdaptiveResize>)
                    _shaderProgram->setUniform("u_Projection", _projection);
            },
            _window->getResizePolicy());

        _shaderProgram->unbind();
    }

private:
    Runic::Window* _window;
    Runic::GraphicsContext* _context;

    std::shared_ptr<Runic::Texture> _texture;
    std::unique_ptr<Runic::Mesh<Pos3Tex2>> _mesh;
    std::shared_ptr<Runic::ShaderProgram> _shaderProgram;

    glm::mat4 _projection{1.0F};

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

    glm::vec3 _cameraPos{0.0F, 0.0F, 3.0F};
    glm::vec3 _cameraFront{0.0F, 0.0F, -1.0F};
    glm::vec3 _cameraUp{0.0F, 1.0F, 0.0F};
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
        layers.push_back(std::make_unique<TriangleLayer>(window, context));
    }
};

// Application -------------------------------------------------------------------------------------

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<Sandbox>();
}
