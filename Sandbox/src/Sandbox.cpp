#include <Runic.hpp>

// Cube --------------------------------------------------------------------------------------------

static constexpr Runic::color Coral(1.0F, 0.5F, 0.31F, 1.0F);

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
        _light.direction = {-0.2F, -1.0F, -0.3F};

        // _cube = Runic::Graphics::Cube::Create(
        //     _context,
        //     Runic::ShaderManager::Find<Runic::Graphics::CubeLayout>(
        //         _context, "Directional", "Flat"));

        // _cube->setMaterial(
        //     "material", Runic::PhongMaterial(Coral, Coral, Runic::Color::White, 23.0F));

        _cube = Runic::Graphics::Cube::Create(
            _context,
            Runic::ShaderManager::Find<Runic::Graphics::CubeLayout>(
                _context, "Directional", "LightMap"));

        _cube->setMaterial(
            "material",
            Runic::LightingMapMaterial(
                _cube->addTexture("container2.png"),
                _cube->addTexture("container2_specular.png"),
                32.0F));

        _cube->rotationAxis = {1.0F, 0.3F, 0.5F};

        _cube->shader->bind();
        _cube->shader->setUniform("light.direction", _light.direction);
        _cube->shader->setUniform("light.ambient", _light.ambient);
        _cube->shader->setUniform("light.diffuse", _light.diffuse);
        _cube->shader->setUniform("light.specular", _light.specular);
        _cube->shader->setUniform("viewPosition", _camera.position());
        _cube->shader->unbind();

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

    void detach() override
    {
        _window->releaseMouse();

        _cube.reset();
    }

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

        _cube->shader->bind();
        _cube->shader->setUniform("viewPosition", _camera.position());
        _cube->shader->unbind();
    }

    void render() override
    {
        _context->setClearColor(Runic::Color::Gray1);
        _context->clear();

        for (int i = 0; i < 10; i++)
        {
            _cube->position = _cubePositions[i]; // NOLINT
            _cube->rotation = 20.0F * static_cast<float>(i);
            _cube->draw(_camera);
        }
    }

private:
    Runic::Light::Directional _light;
    std::unique_ptr<Runic::Graphics::Cube> _cube;

    // clang-format off
    const std::array<glm::vec3, 10> _cubePositions = {
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
    // clang-format off

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
