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

    template <typename EventType>
    void addEventHandler(std::function<bool(const EventType&)> handler)
    {
        _dispatcher.addHandler<EventType>(handler);
    }

    template <typename EventType>
    bool handleEvent(const EventType& event)
    {
        return _dispatcher.dispatch(event);
    }

private:
    std::string _name;
    EventDispatcher _dispatcher;
};

} // namespace Runic
