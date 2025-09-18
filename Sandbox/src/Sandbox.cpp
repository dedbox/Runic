#include <Runic.hpp>

#include "glm/ext/vector_float3.hpp"

// Grass -------------------------------------------------------------------------------------------

class Grass : public Runic::RenderObject
{
private:
    explicit Grass(Runic::GraphicsContext* context)
        : Runic::RenderObject(context)
    {
    }

public:
    static std::unique_ptr<Grass> Create(Runic::GraphicsContext* context)
    {
        auto grass = std::unique_ptr<Grass>(new Grass(context));

        grass->createMesh(Runic::DrawMode::Triangles);

        const std::vector<Runic::VertexAttribute> layout{
            {.type = Runic::AttributeType::Float3, .normalize = false},
            {.type = Runic::AttributeType::Float3, .normalize = false},
            {.type = Runic::AttributeType::Float2, .normalize = false},
        };

        // clang-format off
        const std::vector<float> vertices{
            -0.5F, -0.5F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F,
             0.5F, -0.5F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0.0F,
             0.5F,  0.5F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F,
            -0.5F,  0.5F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F,
        };
        // clang-format on

        const std::vector<uint32_t> indices{0, 1, 2, 2, 3, 0};

        grass->mesh->addVertexBuffer(
            Runic::VertexBuffer::Create(context, vertices, Runic::BufferUsage::Static), layout);
        grass->mesh->setIndices(indices, Runic::IndexType::Int, Runic::BufferUsage::Static);

        grass->addTexture(
            "texture1",
            "textures/grass.png",
            {.wrapS = Runic::TextureWrap::ClampEdge, .wrapT = Runic::TextureWrap::ClampEdge});

        return grass;
    }
};

// MainLayer ---------------------------------------------------------------------------------------

class MainLayer : public Runic::Layer
{
private:
    Runic::Window* _window;
    Runic::GraphicsContext* _context;

public:
    MainLayer(Runic::Window* window, Runic::GraphicsContext* context)
        : _window(window)
        , _context(context)
    {
        _cube = Runic::Graphics::Cube::Create(_context);
        _cube->addTexture("texture1", "textures/marble.jpg", {});

        _plane = Runic::Graphics::Plane::Create(_context);
        _plane->addTexture("texture1", "textures/metal.png", {});
        _plane->scale    = {5.0F, 1.0F, 5.0F};
        _plane->position = {0.0F, -0.501F, 0.0F};

        _grass = Grass::Create(_context);

        _shader = Runic::ShaderManager::Find(_context, "Model", "Discard");
    }

    void attach() override
    {
        std::visit(
            [&](auto&& policy) {
                using T = std::decay_t<decltype(policy)>;
                if constexpr (std::is_same_v<T, Runic::FixedAspect>)
                    _camera.setAspect(policy.aspectRatio);

                else if constexpr (std::is_same_v<T, Runic::AdaptiveResize>)
                {
                    const auto size = _window->getSize();

                    _camera.setAspect(static_cast<float>(size.x) / static_cast<float>(size.y));

                    Runic::EventBus::Subscribe<Runic::WindowResizeEvent>(
                        [&](const Runic::WindowResizeEvent& event) {
                            _camera.setAspect(
                                static_cast<float>(event.width) / static_cast<float>(event.height));
                        });
                }
            },
            _window->getResizePolicy());

        Runic::EventBus::Subscribe<Runic::KeyPressEvent>([&](const Runic::KeyPressEvent& event) {
            if (!_window->isMouseCaptured())
            {
                if (event.key == Runic::Key::Escape)
                    _window->captureMouse();
                return;
            }

            switch (event.key)
            {
            case Runic::Key::Escape:
                _window->releaseMouse();
                _camera.moveSpeed = 1.0F;
                break;

            case Runic::Key::LeftShift:
            case Runic::Key::RightShift:
                _camera.moveSpeed = 5.0F;
                break;

            default:
                break;
            }
        });

        Runic::EventBus::Subscribe<Runic::KeyReleaseEvent>(
            [&](const Runic::KeyReleaseEvent& event) {
                if (!_window->isMouseCaptured())
                    return;

                switch (event.key)
                {
                case Runic::Key::LeftShift:
                case Runic::Key::RightShift:
                    _camera.moveSpeed = 1.0F;
                    break;

                default:
                    break;
                }
            });

        Runic::EventBus::Subscribe<Runic::MouseMoveEvent>([&](const Runic::MouseMoveEvent& event) {
            if (!_window->isMouseCaptured())
                return;

            _camera.rotateHorizontal(event.xOffset);
            _camera.rotateVertical(event.yOffset);
        });

        Runic::EventBus::Subscribe<Runic::MouseScrollEvent>(
            [&](const Runic::MouseScrollEvent& event) {
                if (!_window->isMouseCaptured())
                    return;

                _camera.zoom(event.vert);
            });

        _window->captureMouse();
    }

    void detach() override { _window->releaseMouse(); }

    void update(double deltaTime) override
    {
        const auto amount = static_cast<float>(deltaTime);

        if (_window->isMouseCaptured())
        {
            if (Runic::Input::IsKeyPressed(Runic::Key::W))
                _camera.moveForward(amount);

            if (Runic::Input::IsKeyPressed(Runic::Key::S))
                _camera.moveBackward(amount);

            if (Runic::Input::IsKeyPressed(Runic::Key::A))
                _camera.moveLeft(amount);

            if (Runic::Input::IsKeyPressed(Runic::Key::D))
                _camera.moveRight(amount);

            if (Runic::Input::IsKeyPressed(Runic::Key::Space))
                _camera.moveUp(amount);

            if (Runic::Input::IsKeyPressed(Runic::Key::C))
                _camera.moveDown(amount);
        }
    }

    void render() override
    {
        _context->setClearColor(Runic::Color::Gray1);
        _context->clear();

        _plane->draw(*_shader, _camera);

        _cube->position = {-1.0F, 0.0F, -1.0F};
        _cube->draw(*_shader, _camera);

        _cube->position = {2.0F, 0.0F, 0.0F};
        _cube->draw(*_shader, _camera);

        for (const auto& position : _vegetation)
        {
            _grass->position = position;
            _grass->draw(*_shader, _camera);
        }
    }

private:
    std::unique_ptr<Runic::Graphics::Cube> _cube;
    std::unique_ptr<Runic::Graphics::Plane> _plane;
    std::unique_ptr<Grass> _grass;
    std::shared_ptr<Runic::ShaderProgram> _shader;
    Runic::Camera _camera;

    // clang-format off
    std::vector<glm::vec3> _vegetation = {
        {-1.5F, 0.0F, -0.48F},
        { 1.5F, 0.0F,  0.51F},
        { 0.0F, 0.0F,  0.7F},
        {-0.3F, 0.0F, -2.3F},
        { 0.5F, 0.0F, -0.6F},
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

        layers.push_back(std::make_unique<MainLayer>(window, context));
    }
};

// Application -------------------------------------------------------------------------------------

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<Sandbox>();
}
