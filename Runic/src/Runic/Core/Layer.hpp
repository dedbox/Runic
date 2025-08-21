#pragma once

namespace Runic
{

class Layer
{
public:
    explicit Layer(std::string name = "Layer")
        : name(std::move(name))
    {
    }

    virtual ~Layer() = default;

    Layer(const Layer&)            = delete;
    Layer(Layer&&)                 = delete;
    Layer& operator=(const Layer&) = delete;
    Layer& operator=(Layer&&)      = delete;

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate() {}
    virtual void onGuiRender() {}

    // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    std::string name;
};

} // namespace Runic
