#include <Runic.hpp>

// Cube --------------------------------------------------------------------------------------------

static constexpr Runic::color AlmostBlack(0.1F, 0.1F, 0.1F, 1.0F);
static constexpr Runic::color Coral(1.0F, 0.5F, 0.31F, 1.0F);

using APos3 = Runic::Attribute<float, 3>;
using LPos3 = Runic::Layout<APos3>;

class Cube : public Runic::RenderObject<LPos3>
{
private:
    explicit Cube(Runic::GraphicsContext* context)
        : Runic::RenderObject<LPos3>(context)
    {
    }

public:
    static std::unique_ptr<Cube> Create(
        Runic::GraphicsContext* context, std::shared_ptr<Runic::ShaderProgram<LPos3>>&& shader)
    {
        auto cube = std::unique_ptr<Cube>(new Cube(context));

        cube->createMesh(Runic::DrawMode::Triangles);
        cube->mesh->setVertexBuffers(
            std::make_tuple(
                Runic::VertexBuffer<LPos3>::Create(
                    context,
                    Runic::VertexData<LPos3>(cube->_vertices),
                    Runic::BufferUsage::Static)));

        cube->shader = std::move(shader);

        return cube;
    }

private:
    // clang-format off
    std::vector<float> _vertices = {
            // a_Position
            -0.5F, -0.5F, -0.5F,
             0.5F, -0.5F, -0.5F,
             0.5F,  0.5F, -0.5F,
             0.5F,  0.5F, -0.5F,
            -0.5F,  0.5F, -0.5F,
            -0.5F, -0.5F, -0.5F,

            -0.5F, -0.5F,  0.5F,
             0.5F, -0.5F,  0.5F,
             0.5F,  0.5F,  0.5F,
             0.5F,  0.5F,  0.5F,
            -0.5F,  0.5F,  0.5F,
            -0.5F, -0.5F,  0.5F,

            -0.5F,  0.5F,  0.5F,
            -0.5F,  0.5F, -0.5F,
            -0.5F, -0.5F, -0.5F,
            -0.5F, -0.5F, -0.5F,
            -0.5F, -0.5F,  0.5F,
            -0.5F,  0.5F,  0.5F,

             0.5F,  0.5F,  0.5F,
             0.5F,  0.5F, -0.5F,
             0.5F, -0.5F, -0.5F,
             0.5F, -0.5F, -0.5F,
             0.5F, -0.5F,  0.5F,
             0.5F,  0.5F,  0.5F,

            -0.5F, -0.5F, -0.5F,
             0.5F, -0.5F, -0.5F,
             0.5F, -0.5F,  0.5F,
             0.5F, -0.5F,  0.5F,
            -0.5F, -0.5F,  0.5F,
            -0.5F, -0.5F, -0.5F,

            -0.5F,  0.5F, -0.5F,
             0.5F,  0.5F, -0.5F,
             0.5F,  0.5F,  0.5F,
             0.5F,  0.5F,  0.5F,
            -0.5F,  0.5F,  0.5F,
            -0.5F,  0.5F, -0.5F,
        };
    // clang-format on
};

// CubeLayer --------------------------------------------------------------------------------------

class CubeLayer : public Runic::Layer
{
private:
    Runic::Window* _window;
    Runic::GraphicsContext* _context;

public:
    CubeLayer(Runic::Window* window, Runic::GraphicsContext* context)
        : _window(window)
        , _context(context)
    {
    }

    void attach() override
    {
        _light = Cube::Create(
            _context, Runic::ShaderManager::Find<LPos3>(_context, "Pos3MVP", "LightSource"));

        _light->shader->bind();
        _light->shader->setUniform("u_LightColor", Runic::Color::White);
        _light->shader->unbind();

        _light->position = {1.2F, 1.0F, 2.0F};
        _light->scale    = glm::vec3(0.2F);

        _object = Cube::Create(
            _context, Runic::ShaderManager::Find<LPos3>(_context, "Pos3MVP", "LitObject"));

        _object->shader->bind();
        _object->shader->setUniform("u_LightColor", Runic::Color::White);
        _object->shader->setUniform("u_ObjectColor", Coral);
        _object->shader->unbind();

        std::visit(
            [&](auto&& policy) {
                using T = std::decay_t<decltype(policy)>;
                if constexpr (std::is_same_v<T, Runic::FixedAspect>)
                    _camera.setAspect(policy.aspectRatio);

                else if constexpr (std::is_same_v<T, Runic::AdaptiveResize>)
                {
                    const auto size = _window->getSize();

                    _camera.setAspect(static_cast<float>(size.x) / static_cast<float>(size.y));

                    addEventHandler<Runic::WindowResizeEvent>(
                        [&](const Runic::WindowResizeEvent& event) {
                            _camera.setAspect(
                                static_cast<float>(event.width) / static_cast<float>(event.height));
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

        addEventHandler<Runic::MouseMoveEvent>([&](const auto& event) {
            if (!_window->isMouseCaptured())
                return false;

            _camera.rotateHorizontal(event.xOffset);
            _camera.rotateVertical(event.yOffset);

            return false;
        });

        addEventHandler<Runic::MouseScrollEvent>([&](const auto& event) {
            if (!_window->isMouseCaptured())
                return false;

            _camera.zoom(event.vert);

            return false;
        });

        _window->captureMouse();
    }

    void detach() override
    {
        _window->releaseMouse();

        _light.reset();
        _object.reset();
    }

    void update(double deltaTime) override
    {
        const auto amount = static_cast<float>(deltaTime);

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

    void render() override
    {
        _context->setClearColor(AlmostBlack);
        _context->clear();

        _light->draw(_camera);
        _object->draw(_camera);
    }

private:
    std::unique_ptr<Cube> _light;
    std::unique_ptr<Cube> _object;

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

        layers.push_back(std::make_unique<CubeLayer>(window, context));
    }
};

// Application -------------------------------------------------------------------------------------

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<Sandbox>();
}
