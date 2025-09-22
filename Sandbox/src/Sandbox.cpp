#include <Runic.hpp>

#include "glm/ext/matrix_float4x4.hpp"

constexpr float CameraSneakSpeed = 1.0F;
constexpr float CameraWalkSpeed  = 5.0F;
constexpr float CameraRunSpeed   = 10.0F;

// Sky Box -----------------------------------------------------------------------------------------

class SkyBox : public Runic::RenderObject
{
private:
    explicit SkyBox(Runic::GraphicsContext* context)
        : Runic::RenderObject(context)
    {
    }

public:
    static std::unique_ptr<SkyBox> Create(
        Runic::GraphicsContext* context, const std::shared_ptr<Runic::CubeMap>& cubeMap)
    {
        auto skyBox = std::unique_ptr<SkyBox>(new SkyBox(context));

        skyBox->createMesh(Runic::DrawMode::Triangles);

        const std::vector<Runic::VertexAttribute> layout{
            {.type = Runic::AttributeType::Float3, .normalize = false}, // position
            {.type = Runic::AttributeType::Float3, .normalize = false}, // normal
        };

        // clang-format off
        const std::vector<float> vertices{
            -0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F,
             0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 
             0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 
             0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 
            -0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 
            -0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 

            -0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
             0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
             0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
             0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
            -0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
            -0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,

            -0.5F,  0.5F,  0.5F, -1.0F,  0.0F,  0.0F,
            -0.5F,  0.5F, -0.5F, -1.0F,  0.0F,  0.0F,
            -0.5F, -0.5F, -0.5F, -1.0F,  0.0F,  0.0F,
            -0.5F, -0.5F, -0.5F, -1.0F,  0.0F,  0.0F,
            -0.5F, -0.5F,  0.5F, -1.0F,  0.0F,  0.0F,
            -0.5F,  0.5F,  0.5F, -1.0F,  0.0F,  0.0F,

             0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  0.0F,
             0.5F,  0.5F, -0.5F,  1.0F,  0.0F,  0.0F,
             0.5F, -0.5F, -0.5F,  1.0F,  0.0F,  0.0F,
             0.5F, -0.5F, -0.5F,  1.0F,  0.0F,  0.0F,
             0.5F, -0.5F,  0.5F,  1.0F,  0.0F,  0.0F,
             0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  0.0F,

            -0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,
             0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,
             0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,
             0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,
            -0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,
            -0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,

            -0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,
             0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,
             0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,
             0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,
            -0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,
            -0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,
        };
        // clang-format on

        skyBox->mesh->addVertexBuffer(
            Runic::VertexBuffer::Create(context, vertices, Runic::BufferUsage::Static), layout);
        skyBox->mesh->addCubeMap("skyBox", cubeMap);

        return skyBox;
    }
};

class SkyBoxCamera : public Runic::Camera
{
public:
    explicit SkyBoxCamera(const Camera& camera)
        : Camera(camera)
    {
    }

    glm::mat4 viewMatrix() const override { return glm::mat4(glm::mat3(Camera::viewMatrix())); }
};

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
        Runic::GraphicsContext* context, const std::shared_ptr<Runic::Texture>& texture)
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

        _screenShader = Runic::ShaderManager::Find(_context, "Screen", "Screen");

        auto cubeMap = Runic::TextureManager::FindCubeMap(
            _context,
            {
                "cubemaps/skybox/right.jpg",
                "cubemaps/skybox/left.jpg",
                "cubemaps/skybox/top.jpg",
                "cubemaps/skybox/bottom.jpg",
                "cubemaps/skybox/front.jpg",
                "cubemaps/skybox/back.jpg",
            });

        _skyBox = SkyBox::Create(_context, cubeMap);

        _skyBoxShader = Runic::ShaderManager::Find(_context, "SkyBox", "SkyBox");

        _backpack = Runic::Model::Create(_context, "models/backpack/backpack.obj");
        _backpack->addCubeMap("skyBox", cubeMap);

        _shader = Runic::ShaderManager::Find(_context, "EnvReflect", "EnvReflect");
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
                _camera.moveSpeed = CameraWalkSpeed;
                break;

            case Runic::Key::LeftShift:
            case Runic::Key::RightShift:
                _camera.moveSpeed = CameraRunSpeed;
                break;

            case Runic::Key::LeftCtrl:
            case Runic::Key::RightCtrl:
                _camera.moveSpeed = CameraSneakSpeed;
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
                case Runic::Key::LeftCtrl:
                case Runic::Key::RightCtrl:
                    _camera.moveSpeed = CameraWalkSpeed;
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

        _camera.moveSpeed = CameraWalkSpeed;

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

        _shader->bind();
        _shader->setUniform("cameraPosition", _camera.position());
        _shader->unbind();

        _backpack->draw(*_shader, _camera);

        SkyBoxCamera skyCam(_camera);

        _context->setDepthFunction(Runic::DepthFunction::LEqual);
        _skyBox->draw(*_skyBoxShader, skyCam);
        _context->setDepthFunction(Runic::DepthFunction::Less);

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

    std::unique_ptr<SkyBox> _skyBox;
    std::shared_ptr<Runic::ShaderProgram> _skyBoxShader;

    std::unique_ptr<Runic::Model> _backpack;
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
