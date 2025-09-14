#include <Runic.hpp>

#include "glm/trigonometric.hpp"

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
        _dirLight.direction = {-0.2F, -1.0F, -0.3F};
        _dirLight.ambient   = {0.05F, 0.05F, 0.05F, 1.0F};
        _dirLight.diffuse   = {0.4F, 0.4F, 0.4F, 1.0F};
        _dirLight.specular  = {0.5F, 0.5F, 0.5F, 1.0F};

        _pointLight.ambient   = {0.05F, 0.05F, 0.05F, 1.0F};
        _pointLight.diffuse   = {0.8F, 0.8F, 0.8F, 1.0F};
        _pointLight.specular  = {1.0F, 1.0F, 1.0F, 1.0F};
        _pointLight.constant  = 1.0F;
        _pointLight.linear    = 0.09F;
        _pointLight.quadratic = 0.032F;

        _spotLight.cutoff      = glm::cos(glm::radians(12.5F));
        _spotLight.outerCutoff = glm::cos(glm::radians(15.0F));
        _spotLight.ambient     = {0.0F, 0.0F, 0.0F, 1.0F};
        _spotLight.diffuse     = {1.0F, 1.0F, 1.0F, 1.0F};
        _spotLight.specular    = {1.0F, 1.0F, 1.0F, 1.0F};
        _spotLight.constant    = 1.0F;
        _spotLight.linear      = 0.09F;
        _spotLight.quadratic   = 0.032F;

        _lightCube = Runic::Graphics::Cube::Create(
            _context,
            Runic::ShaderManager::Find<Runic::Graphics::CubeLayout>(_context, "Light", "Light"));

        _lightCube->scale = glm::vec3(0.1);

        _lightCube->shader->bind();
        _lightCube->shader->setUniform("light_color", Runic::Color::White);
        _lightCube->shader->unbind();

        // _cube = Runic::Graphics::Cube::Create(
        //     _context,
        //     Runic::ShaderManager::Find<Runic::Graphics::CubeLayout>(_context, "Object", "Flat"));

        // _cube->setMaterial(
        //     "material", Runic::PhongMaterial(Coral, Coral, Runic::Color::White, 23.0F));

        _cube = Runic::Graphics::Cube::Create(
            _context,
            Runic::ShaderManager::Find<Runic::Graphics::CubeLayout>(
                _context, "Object", "LightMap"));

        _cube->setMaterial(
            "material",
            Runic::LightingMapMaterial(
                _cube->addTexture("container2.png"),
                _cube->addTexture("container2_specular.png"),
                32.0F));

        _cube->rotationAxis = {1.0F, 0.3F, 0.5F};

        _cube->shader->bind();

        _cube->shader->setUniform("dirLight.direction", _dirLight.direction);
        _cube->shader->setUniform("dirLight.ambient", _dirLight.ambient);
        _cube->shader->setUniform("dirLight.diffuse", _dirLight.diffuse);
        _cube->shader->setUniform("dirLight.specular", _dirLight.specular);

        for (int i = 0; i < 4; i++)
        {
            _cube->shader->setUniform(
                std::format("pointLights[{}].position", i), _pointLightPositions[i]); // NOLINT
            _cube->shader->setUniform(
                std::format("pointLights[{}].ambient", i), _pointLight.ambient);
            _cube->shader->setUniform(
                std::format("pointLights[{}].diffuse", i), _pointLight.diffuse);
            _cube->shader->setUniform(
                std::format("pointLights[{}].specular", i), _pointLight.specular);
            _cube->shader->setUniform(
                std::format("pointLights[{}].constant", i), _pointLight.constant);
            _cube->shader->setUniform(std::format("pointLights[{}].linear", i), _pointLight.linear);
            _cube->shader->setUniform(
                std::format("pointLights[{}].quadratic", i), _pointLight.quadratic);
        }

        _cube->shader->setUniform("spotLight.cutoff", _spotLight.cutoff);
        _cube->shader->setUniform("spotLight.outerCutoff", _spotLight.outerCutoff);
        _cube->shader->setUniform("spotLight.ambient", _spotLight.ambient);
        _cube->shader->setUniform("spotLight.diffuse", _spotLight.diffuse);
        _cube->shader->setUniform("spotLight.specular", _spotLight.specular);
        _cube->shader->setUniform("spotLight.constant", _spotLight.constant);
        _cube->shader->setUniform("spotLight.linear", _spotLight.linear);
        _cube->shader->setUniform("spotLight.quadratic", _spotLight.quadratic);

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

        _spotLight.position  = _camera.position();
        _spotLight.direction = _camera.front();

        _cube->shader->bind();

        _cube->shader->setUniform("spotLight.position", _spotLight.position);
        _cube->shader->setUniform("spotLight.direction", _spotLight.direction);

        _cube->shader->setUniform("viewPosition", _camera.position());

        _cube->shader->unbind();
    }

    void render() override
    {
        _context->setClearColor(Runic::Color::Gray1);
        _context->clear();

        for (int i = 0; i < 4; i++)
        {
            _lightCube->position = _pointLightPositions[i]; // NOLINT
            _lightCube->draw(_camera);
        }

        for (int i = 0; i < 10; i++)
        {
            _cube->position = _cubePositions[i]; // NOLINT
            _cube->rotation = 20.0F * static_cast<float>(i);
            _cube->draw(_camera);
        }
    }

private:
    Runic::Light::Directional _dirLight;
    Runic::Light::Point _pointLight;
    Runic::Light::Spot _spotLight;

    std::unique_ptr<Runic::Graphics::Cube> _lightCube;
    std::unique_ptr<Runic::Graphics::Cube> _cube;

    // clang-format off
    std::array<glm::vec3, 4> _pointLightPositions = {
        glm::vec3( 0.7F,  0.2F,   2.0F),
        glm::vec3( 2.3F, -3.3F,  -4.0F),
        glm::vec3(-4.0F,  2.0F, -12.0F),
        glm::vec3( 0.0F,  0.0F,  -3.0F),
    };

    std::array<glm::vec3, 10> _cubePositions = {
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
