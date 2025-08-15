#pragma once

#include "pch.hpp"
#include "Runic/Layer.hpp"

namespace Runic
{
class RUNIC_API LayerStack
{
public:
    LayerStack();

    ~LayerStack();

    void pushLayer(Layer* layer);

    void popLayer(Layer* layer);

    void pushOverlay(Layer* overlay);

    void popOverlay(Layer* overlay);

    std::vector<Layer*>::iterator begin() { return _layers.begin(); }
    std::vector<Layer*>::iterator end() { return _layers.end(); }

private:
    std::vector<Layer*> _layers;
    unsigned int _layerInsertIndex{0};
};
} // Runic
