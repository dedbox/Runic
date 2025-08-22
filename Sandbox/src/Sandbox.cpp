#include <Runic.hpp>

class ClearLayer : public Runic::Layer
{
public:
    explicit ClearLayer(Runic::GraphicsContext& context, const glm::vec4& color)
        : _context(context)
        , _color(color)
    {
    }

    void onUpdate() override
    {
        _context.setClearColor(_color);
        _context.clear();
    }

private:
    Runic::GraphicsContext& _context;
    const glm::vec4 _color;
};

class SandboxApplication : public Runic::Application
{
public:
    SandboxApplication()
        : Application({.name = "Sandbox"})
    {
        auto context                = getGraphicsContext();
        Runic::LayerManager& layers = getLayerManager();
        layers.push_back(std::make_unique<ClearLayer>(context, glm::vec4(0.0F, 1.0F, 0.0F, 1.0F)));
    }
};

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<SandboxApplication>();
}
