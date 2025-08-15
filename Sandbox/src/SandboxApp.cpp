#include <Runic.hpp>

class SandboxApp final : public Runic::Application
{
public:
    SandboxApp() {}

    ~SandboxApp() override = default;
};

Runic::Application* Runic::CreateApplication()
{
    return new SandboxApp();
}
