#pragma once

#include <Runic.hpp>

constexpr float CameraSneakSpeed = 1.0F;
constexpr float CameraWalkSpeed  = 5.0F;
constexpr float CameraRunSpeed   = 10.0F;

class MainLayerBase : public Runic::Layer
{
private:
    Runic::Window* _window;
    Runic::GraphicsContext* _context;

public:
    MainLayerBase(Runic::Window* window, Runic::GraphicsContext* context)
        : _window(window)
        , _context(context)
    {
    }

    void attach() override
    {
        // Configure the camera
        std::visit(
            [&](auto&& policy) {
                using T = std::decay_t<decltype(policy)>;

                // For fixed aspect, we only need set the camera aspect once.
                if constexpr (std::is_same_v<T, Runic::FixedAspect>)
                    _camera.setAspect(policy.aspectRatio);

                // For adaptive resize, we must update the camera aspect whenever the window is
                // resized.
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

        // Key Press

        Runic::EventBus::Subscribe<Runic::KeyPressEvent>([&](const Runic::KeyPressEvent& event) {
            // Caopture Mouse
            if (!_window->isMouseCaptured())
            {
                if (event.key == Runic::Key::Escape)
                    _window->captureMouse();
                return;
            }

            switch (event.key)
            {
                // Release mouse capture
            case Runic::Key::Escape:
                _window->releaseMouse();
                _camera.moveSpeed = CameraWalkSpeed;
                break;

                // Begin Run
            case Runic::Key::LeftShift:
            case Runic::Key::RightShift:
                _camera.moveSpeed = CameraRunSpeed;
                break;

                // Begin Sneak
            case Runic::Key::LeftCtrl:
            case Runic::Key::RightCtrl:
                _camera.moveSpeed = CameraSneakSpeed;
                break;

            default:
                break;
            }
        });

        // Key Release

        Runic::EventBus::Subscribe<Runic::KeyReleaseEvent>(
            [&](const Runic::KeyReleaseEvent& event) {
                if (!_window->isMouseCaptured())
                    return;

                switch (event.key)
                {
                    // End Sneak / Run
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

        // Mouse Move

        Runic::EventBus::Subscribe<Runic::MouseMoveEvent>([&](const Runic::MouseMoveEvent& event) {
            if (!_window->isMouseCaptured())
                return;

            _camera.rotateHorizontal(event.xOffset);
            _camera.rotateVertical(event.yOffset);
        });

        // Mouse Scroll

        Runic::EventBus::Subscribe<Runic::MouseScrollEvent>(
            [&](const Runic::MouseScrollEvent& event) {
                if (!_window->isMouseCaptured())
                    return;

                _camera.zoom(event.vert);
            });

        // initial camera speed
        _camera.moveSpeed = CameraWalkSpeed;

        // capture mouse on attach
        _window->captureMouse();
    }

    void detach() override { _window->releaseMouse(); }

    void update(double deltaTime) override
    {
        const auto amount = static_cast<float>(deltaTime);

        if (_window->isMouseCaptured())
        {
            // Move Forward
            if (Runic::Input::IsKeyPressed(Runic::Key::W))
                _camera.moveForward(amount);

            // Move Backward
            if (Runic::Input::IsKeyPressed(Runic::Key::S))
                _camera.moveBackward(amount);

            // Move Left
            if (Runic::Input::IsKeyPressed(Runic::Key::A))
                _camera.moveLeft(amount);

            // Move Right
            if (Runic::Input::IsKeyPressed(Runic::Key::D))
                _camera.moveRight(amount);

            // Move Up
            if (Runic::Input::IsKeyPressed(Runic::Key::Space))
                _camera.moveUp(amount);

            // Move Down
            if (Runic::Input::IsKeyPressed(Runic::Key::C))
                _camera.moveDown(amount);
        }
    }

private:
    Runic::Camera _camera;
};
