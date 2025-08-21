#pragma once

#include "Event.hpp"

namespace Runic
{

class Layer
{
public:
    explicit Layer(std::string name = "Layer")
        : _name(std::move(name))
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

    [[nodiscard]] const std::string& getName() const { return _name; }

private:
    std::string _name;

private:
    EventDispatcher _dispatcher;
};

} // namespace Runic
