#pragma once

#include "Layer.hpp"

namespace Runic
{

class LayerManager
{
public:
    LayerManager() = default;

    ~LayerManager() = default;

    LayerManager(LayerManager&&)                = delete;
    LayerManager(const LayerManager&)           = delete;
    LayerManager operator=(LayerManager&&)      = delete;
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

    [[nodiscard]] auto begin() const { return _layers.begin(); }
    [[nodiscard]] auto end() const { return _layers.end(); }

private:
    std::list<std::unique_ptr<Layer>> _layers;
    std::optional<std::list<std::unique_ptr<Layer>>::iterator> _sep;
};

} // namespace Runic
