#include <Runic.hpp>

static constexpr auto Taupe  = Runic::Color::hex(0x463F3AFF);
static constexpr auto Gray   = Runic::Color::hex(0x8A817CFF);
static constexpr auto Silver = Runic::Color::hex(0xBCB8B1FF);
static constexpr auto Ivory  = Runic::Color::hex(0xF4F3EEFF);
static constexpr auto Melon  = Runic::Color::hex(0xE0AFA0FF);

class SandboxApplication : public Runic::Application
{
public:
    SandboxApplication()
        : Application({.name = "Sandbox"})
        , _renderer(getRenderer())
    {
    }

    void onUpdate() override
    {
        _renderer.beginFrame();
        _renderer.setClearColor(Taupe);
        _renderer.clear();
        _renderer.endFrame();
    }

private:
    Runic::Renderer& _renderer;
};

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<SandboxApplication>();
}
