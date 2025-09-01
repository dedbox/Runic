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

    // allow moving
    Layer(Layer&&)            = default;
    Layer& operator=(Layer&&) = default;

    // prevent copying
    Layer(const Layer&)            = delete;
    Layer& operator=(const Layer&) = delete;

    virtual void attach() {}
    virtual void detach() {}
    virtual void update() {}
    virtual void render() {}

    const std::string& getName() const { return _name; }

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
