#include "pch.hpp"
#include "Runic/LayerStack.hpp"

namespace Runic
{
LayerStack::LayerStack() {}

LayerStack::~LayerStack()
{
    for (const Layer* layer : _layers)
        delete layer;
}

void LayerStack::pushLayer(Layer* layer)
{
    _layers.emplace(_layers.begin() + _layerInsertIndex, layer);
    _layerInsertIndex++;
    layer->onAttach();
}

void LayerStack::popLayer(Layer* layer)
{
    const auto it = std::ranges::find(_layers, layer);
    if (it != _layers.end()) {
        layer->onDetach();
        _layers.erase(it);
        _layerInsertIndex--;
    }
}

void LayerStack::pushOverlay(Layer* overlay)
{
    _layers.emplace_back(overlay);
    overlay->onAttach();
}

void LayerStack::popOverlay(Layer* overlay)
{
    const auto it = std::ranges::find(_layers, overlay);
    if (it != _layers.end()) {
        overlay->onDetach();
        _layers.erase(it);
    }
}
} // Runic
