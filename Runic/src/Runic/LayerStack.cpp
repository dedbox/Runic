#include "Runic/LayerStack.hpp"

namespace Runic
{
LayerStack::LayerStack()
{
    _layerInsert = _layers.begin();
}

LayerStack::~LayerStack()
{
    for (const Layer* layer : _layers)
        delete layer;
}

void LayerStack::pushLayer(Layer* layer)
{
    _layerInsert = _layers.emplace(_layerInsert, layer);
    layer->onAttach();
}

void LayerStack::popLayer(Layer* layer)
{
    const auto it = std::ranges::find(_layers, layer);
    if (it != _layers.end()) {
        layer->onDetach();
        _layers.erase(it);
        --_layerInsert;
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
