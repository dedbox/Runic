#include <Runic.hpp>

class SandboxApp final : public Runic::Application
{
public:
    SandboxApp()
    {
        pushOverlay(new Runic::ImGuiLayer());
    }

    ~SandboxApp() override = default;
};

Runic::Application* Runic::CreateApplication()
{
    return new SandboxApp();
}
