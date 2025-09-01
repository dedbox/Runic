#pragma once

#include "Layer.hpp"

namespace Runic
{

class LayerManager
{
public:
    LayerManager()  = default;
    ~LayerManager() = default;

    // allow moving
    LayerManager(LayerManager&&)            = default;
    LayerManager& operator=(LayerManager&&) = default;

    // prevent copying
    LayerManager(const LayerManager&)           = delete;
    LayerManager operator=(const LayerManager&) = delete;

    void push_front(std::unique_ptr<Layer> layer);
    void push_back(std::unique_ptr<Layer> layer);

    std::unique_ptr<Layer> pop_front();
    std::unique_ptr<Layer> pop_back();

    void pushGui_front(std::unique_ptr<Layer> layer);
    void pushGui_back(std::unique_ptr<Layer> layer);

    std::unique_ptr<Layer> popGui_front();
    std::unique_ptr<Layer> popGui_back();

    auto begin() { return _layers.begin(); }
    auto end() { return _layers.end(); }

    auto begin() const { return _layers.begin(); }
    auto end() const { return _layers.end(); }

    auto rbegin() { return _layers.rbegin(); }
    auto rend() { return _layers.rend(); }

    auto rbegin() const { return _layers.rbegin(); }
    auto rend() const { return _layers.rend(); }

private:
    std::list<std::unique_ptr<Layer>> _layers;
    std::optional<std::list<std::unique_ptr<Layer>>::iterator> _sep;
};

} // namespace Runic
