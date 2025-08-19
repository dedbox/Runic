#include <Runic.hpp>

class SandboxApplication : public Runic::Application
{
};

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<SandboxApplication>();
}
