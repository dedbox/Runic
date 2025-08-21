#include <Runic.hpp>

class KeyLayer : public Runic::Layer
{
public:
    explicit KeyLayer(const std::string& loc)
        : Runic::Layer(loc)
    {
        addEventHandler<Runic::KeyPressEvent>(
            [loc](const Runic::KeyPressEvent& event)
            {
                if (!event.repeat) RUNIC_INFO(loc);
                return false;
            });
    }
};

class SandboxApplication : public Runic::Application
{
public:
    SandboxApplication()
    {
        Runic::LayerManager& layers = getLayerManager();

        layers.pushGui_back(std::make_unique<KeyLayer>("X"));
        layers.pushGui_back(std::make_unique<KeyLayer>("Y"));
        layers.pushGui_back(std::make_unique<KeyLayer>("Z"));

        layers.push_back(std::make_unique<KeyLayer>(""));
        layers.push_back(std::make_unique<KeyLayer>("A"));
        layers.push_back(std::make_unique<KeyLayer>("B"));
        layers.push_back(std::make_unique<KeyLayer>("C"));
    }
};

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<SandboxApplication>();
}
