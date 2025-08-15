#pragma once

#include "pch.hpp"
#include "Runic/Event/Event.hpp"

namespace Runic
{
class RUNIC_API Layer
{
public:
    explicit Layer(const std::string& name = "Layer");

    virtual ~Layer() = default;

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate() {}
    virtual void onImGuiRender() {}
    virtual void onEvent(Event& event) {}

    const std::string& getName() const { return _debugName; }

protected:
    std::string _debugName;
};
} // Runic
