#include <Runic.hpp>

class SandboxApplication : public Runic::Application
{
public:
    SandboxApplication()
    {
        // Window Events -------------------------------------------------------

        addEventHandler<Runic::WindowCloseEvent>(
            [](const auto& /*event*/)
            {
                RUNIC_INFO("quit");
                return false;
            });

        addEventHandler<Runic::WindowFocusEvent>(
            [](const auto& /*event*/)
            {
                RUNIC_INFO("focus");
                return true;
            });

        addEventHandler<Runic::WindowResizeEvent>(
            [](const auto& event)
            {
                RUNIC_INFO("resize {}x{}", event.width, event.height);
                return true;
            });

        addEventHandler<Runic::WindowUnfocusEvent>(
            [](const auto& /*event*/)
            {
                RUNIC_INFO("unfocus");
                return true;
            });

        // Key Events ----------------------------------------------------------

        addEventHandler<Runic::KeyPressEvent>(
            [](const auto& event)
            {
                RUNIC_INFO("key press {}{}", event.key, event.repeat ? " (repeat)" : "");
                return true;
            });

        addEventHandler<Runic::KeyReleaseEvent>(
            [](const auto& event)
            {
                RUNIC_INFO("key release {}", event.key);
                return true;
            });

        // Mouse Events --------------------------------------------------------

        addEventHandler<Runic::MouseButtonPressEvent>(
            [](const auto& event)
            {
                RUNIC_INFO("mouse button press {} ({}, {})", event.button, event.x, event.y);
                return true;
            });

        addEventHandler<Runic::MouseButtonReleaseEvent>(
            [](const auto& event)
            {
                RUNIC_INFO("mouse button release {} ({}, {})", event.button, event.x, event.y);
                return true;
            });

        addEventHandler<Runic::MouseMoveEvent>(
            [](const auto& event)
            {
                RUNIC_INFO("mouse move {} {}", event.xOffset, event.yOffset);
                return true;
            });

        addEventHandler<Runic::MouseScrollEvent>(
            [](const auto& event)
            {
                RUNIC_INFO("mouse scroll {} {}", event.horiz, event.vert);
                return true;
            });
    }
};

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<SandboxApplication>();
}
