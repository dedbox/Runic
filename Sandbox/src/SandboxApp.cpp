#include <Runic.hpp>

class Sandbox : public Runic::Application
{
public:
    Sandbox()
    {
        pushOverlay(new Runic::ImGuiLayer());
    }

    ~Sandbox() override = default;
};

Runic::Application* Runic::CreateApplication()
{
    return new Sandbox();
}
