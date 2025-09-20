#include <Runic.hpp>

// Screen ------------------------------------------------------------------------------------------

class Screen : public Runic::RenderObject
{
private:
    explicit Screen(Runic::GraphicsContext* context)
        : Runic::RenderObject(context)
    {
    }

public:
    static std::unique_ptr<Screen> Create(
        Runic::GraphicsContext* context, const std::shared_ptr<Runic::Texture> texture)
    {
        auto screen = std::unique_ptr<Screen>(new Screen(context));

        screen->createMesh(Runic::DrawMode::Triangles);

        const std::vector<Runic::VertexAttribute> layout{
            {.type = Runic::AttributeType::Float2, .normalize = false},
            {.type = Runic::AttributeType::Float2, .normalize = false},
        };

        // clang-format off
        const std::vector<float> vertices{
            -1.0F,  1.0F, 0.0F, 1.0F,
            -1.0F, -1.0F, 0.0F, 0.0F,
             1.0F, -1.0F, 1.0F, 0.0F,
             1.0F,  1.0F, 1.0F, 1.0F,
        };
        // clang-format on

        const std::vector<uint32_t> indices{0, 1, 2, 0, 2, 3};

        screen->mesh->addVertexBuffer(
            Runic::VertexBuffer::Create(context, vertices, Runic::BufferUsage::Static), layout);
        screen->mesh->setIndices(indices, Runic::IndexType::Int, Runic::BufferUsage::Static);
        screen->mesh->addTexture("screenTexture", texture);

        return screen;
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
        auto size    = _window->getSize();
        _frameBuffer = Runic::FrameBuffer::Create(_context, size.x, size.y);
        _frameBuffer->setRenderBuffer(Runic::RenderBuffer::Create(_context, size.x, size.y));
        _frameBuffer->unbind();

        _screen = Screen::Create(_context, _frameBuffer->getTexture());

        _screenShader = Runic::ShaderManager::Find(_context, "Screen", "EdgeDetect");

        _context->enableBlend();
        _context->setBlendFunction(
            Runic::BlendFactor::SrcAlpha, Runic::BlendFactor::OneMinusSrcAlpha);

        _cube = Runic::Graphics::Cube::Create(_context);
        _cube->addTexture("texture1", "textures/container.jpg", {});

        _plane = Runic::Graphics::Plane::Create(_context);
        _plane->addTexture("texture1", "textures/metal.png", {});
        _plane->scale    = {5.0F, 1.0F, 5.0F};
        _plane->position = {0.0F, -0.501F, 0.0F};

        _shader = Runic::ShaderManager::Find(_context, "Model", "Discard");
    }

    void attach() override
    {
        Runic::EventBus::Subscribe<Runic::WindowResizeEvent>(
            [&](const Runic::WindowResizeEvent& event) {
                int width    = static_cast<int>(event.width);
                int height   = static_cast<int>(event.height);
                _frameBuffer = Runic::FrameBuffer::Create(_context, width, height);
                _frameBuffer->setRenderBuffer(Runic::RenderBuffer::Create(_context, width, height));
                _frameBuffer->unbind();
                _screen = Screen::Create(_context, _frameBuffer->getTexture());
            });

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

            case Runic::Key::P: {
                if (_wireframe)
                    _context->setPolygonMode(Runic::PolygonMode::Fill);
                else
                    _context->setPolygonMode(Runic::PolygonMode::Line);
                _wireframe = !_wireframe;
                break;
            }

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

        _screenShader->bind();
        _screenShader->setUniform("wireframe", _wireframe);
        _screenShader->unbind();
    }

    void render() override
    {
        // scene
        _frameBuffer->bind();

        _context->setClearColor(Runic::Color::Gray1);
        _context->clear();

        _plane->draw(*_shader, _camera);

        _cube->position = {-1.0F, 0.0F, -1.0F};
        _cube->draw(*_shader, _camera);

        _cube->position = {2.0F, 0.0F, 0.0F};
        _cube->draw(*_shader, _camera);

        _frameBuffer->unbind();

        // screen

        _context->setClearColor(Runic::Color::Blue);
        _context->clear();

        _screen->draw(*_screenShader);
    }

private:
    std::unique_ptr<Runic::FrameBuffer> _frameBuffer;
    std::shared_ptr<Runic::ShaderProgram> _screenShader;
    std::unique_ptr<Screen> _screen;
    bool _wireframe = false;

    std::unique_ptr<Runic::Graphics::Cube> _cube;
    std::unique_ptr<Runic::Graphics::Plane> _plane;
    std::shared_ptr<Runic::ShaderProgram> _shader;
    Runic::Camera _camera;
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
