#include <Runic.hpp>

class Sandbox : public Runic::Application
{
public:
    Sandbox() = default;

    ~Sandbox() override = default;
};

Runic::Application* Runic::CreateApplication()
{
    return new Sandbox();
}
