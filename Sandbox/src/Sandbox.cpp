#include <Runic.hpp>

class SandboxApplication : public Runic::Application
{
public:
    void onUpdate() override
    {
        // Keys ----------------------------------------------------------------

        std::vector<std::string> keys;

        if (Runic::Input::IsKeyPressed(Runic::Key::Escape)) keys.emplace_back("Escape");
        if (Runic::Input::IsKeyPressed(Runic::Key::Space)) keys.emplace_back("Space");

        if (!keys.empty())
            RUNIC_INFO(
                "DOWN: " +
                (keys | std::ranges::views::join_with('|') | std::ranges::to<std::string>()));

        // Mouse Buttons -------------------------------------------------------

        std::vector<std::string> buttons;
        if (Runic::Input::IsMouseButtonPressed(Runic::MouseButton::Left))
            buttons.emplace_back("Left");
        if (Runic::Input::IsMouseButtonPressed(Runic::MouseButton::Middle))
            buttons.emplace_back("Middle");
        if (Runic::Input::IsMouseButtonPressed(Runic::MouseButton::Right))
            buttons.emplace_back("Right");
        if (Runic::Input::IsMouseButtonPressed(Runic::MouseButton::X1)) buttons.emplace_back("X1");
        if (Runic::Input::IsMouseButtonPressed(Runic::MouseButton::X2)) buttons.emplace_back("X2");

        if (!buttons.empty())
            RUNIC_INFO(
                "PRESSED: " +
                (buttons | std::ranges::views::join_with('|') | std::ranges::to<std::string>()));
    }
};

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<SandboxApplication>();
}
