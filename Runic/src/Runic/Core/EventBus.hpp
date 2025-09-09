#pragma once

#include "Runic/Core/Event.hpp"

namespace Runic
{

class EventBus
{
public:
    template <typename EventType>
    static void Subscribe(std::function<void(const EventType&)> handler)
    {
        GetInstance().subscribe(handler);
    }

    static void Publish(const EventVariant& event) { GetInstance().publish(event); }

private:
    std::unordered_map<size_t, std::vector<std::function<void(const EventVariant&)>>> _handlers;

    static EventBus& GetInstance()
    {
        static EventBus instance;
        return instance;
    }

    template <typename EventType>
    void subscribe(std::function<void(const EventType&)> handler)
    {
        _handlers[typeid(EventType).hash_code()].push_back([handler](const EventVariant& event) {
            if (const auto* concreteEvent = std::get_if<EventType>(&event))
                handler(*concreteEvent);
        });
    }

    void publish(const EventVariant& event)
    {
        size_t event_type_code =
            std::visit([](const auto& e) { return typeid(e).hash_code(); }, event);

        if (auto it = _handlers.find(event_type_code); it != _handlers.end())
            for (const auto& handler : it->second)
                handler(event);
    }
};

} // namespace Runic
