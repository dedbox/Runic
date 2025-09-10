#include <Runic.hpp>

#include "Cube.hpp"

// Cube --------------------------------------------------------------------------------------------

static constexpr Runic::color AlmostBlack(0.1F, 0.1F, 0.1F, 1.0F);
static constexpr Runic::color DarkGray(0.2F, 0.2F, 0.2F, 1.0F);
static constexpr Runic::color Gray(0.5F, 0.5F, 0.5F, 1.0F);
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
        _light = Cube::Create(
            _context, Runic::ShaderManager::Find<CubeLayout>(_context, "Cube", "LightSource"));

        _light->shader->bind();
        _light->shader->setUniform("u_LightColor", Runic::Color::White);
        _light->shader->unbind();

        _light->position = {1.2F, 1.0F, 2.0F};
        _light->scale    = glm::vec3(0.2F);

        _object = Cube::Create(
            _context, Runic::ShaderManager::Find<CubeLayout>(_context, "Cube", "Material"));

        _object->shader->bind();
        _object->shader->setUniform("u_Material.ambient", Coral);
        _object->shader->setUniform("u_Material.diffuse", Coral);
        _object->shader->setUniform("u_Material.specular", Gray);
        _object->shader->setUniform("u_Material.shininess", 32.0F);
        _object->shader->setUniform("u_Light.position", _light->position);
        _object->shader->setUniform("u_Light.ambient", DarkGray);
        _object->shader->setUniform("u_Light.diffuse", Gray);
        _object->shader->setUniform("u_Light.specular", Runic::Color::White);
        _object->shader->setUniform("u_ViewPosition", _camera.position());
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

        _light.reset();
        _object.reset();
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

        double secs = Runic::Time::Seconds();

        Runic::color lightColor;
        lightColor.r = static_cast<float>(sin(secs * 2.0F));
        lightColor.g = static_cast<float>(sin(secs * 0.7F));
        lightColor.b = static_cast<float>(sin(secs * 1.3F));
        lightColor.a = 1.0F;

        Runic::color diffuseColor = lightColor * glm::vec4(glm::vec3(0.5F), 1.0F);
        Runic::color ambientColor = diffuseColor * glm::vec4(glm::vec3(0.2F), 1.0F);

        _light->shader->bind();
        _light->shader->setUniform("u_LightColor", lightColor);
        _light->shader->unbind();

        _object->shader->bind();
        _object->shader->setUniform("u_Light.ambient", ambientColor);
        _object->shader->setUniform("u_Light.diffuse", diffuseColor);
        _object->shader->setUniform("u_ViewPosition", _camera.position());
        _object->shader->unbind();
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
