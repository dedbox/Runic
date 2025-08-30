#include "LayerManager.hpp"

namespace Runic
{

void LayerManager::push_front(std::unique_ptr<Layer> layer)
{
    layer->attach();

    _layers.push_front(std::move(layer));

    if (!_sep)
    {
        _sep.emplace(_layers.begin());
    }
}

void LayerManager::pushGui_front(std::unique_ptr<Layer> layer)
{
    layer->attach();

    if (_sep)
        _layers.insert(std::next(*_sep), std::move(layer));
    else
        _layers.push_front(std::move(layer));
}

void LayerManager::push_back(std::unique_ptr<Layer> layer)
{
    layer->attach();

    if (_sep)
    {
        _layers.insert(std::next(*_sep), std::move(layer));
        _sep.emplace(std::next(*_sep));
    }
    else
    {
        _layers.push_front(std::move(layer));
        _sep.emplace(_layers.begin());
    }
}

void LayerManager::pushGui_back(std::unique_ptr<Layer> layer)
{
    layer->attach();

    _layers.push_back(std::move(layer));
}

std::unique_ptr<Layer> LayerManager::pop_front()
{
    if (!_sep)
        throw std::runtime_error("Attempted to pop an empty list head");

    auto layer = std::move(_layers.front());

    layer->detach();

    if (*_sep == _layers.begin())
    {
        _layers.pop_front();
        _sep.reset();
    }
    else
        _layers.pop_front();

    return layer;
}

std::unique_ptr<Layer> LayerManager::popGui_front()
{
    if (_layers.empty() || (_sep && *_sep == std::prev(_layers.end())))
        throw std::runtime_error("Attempted to pop an empty list tail");

    std::unique_ptr<Layer> layer;

    if (_sep)
    {
        layer = std::move(*std::next(*_sep));
        _layers.erase(std::next(*_sep));
    }
    else
    {
        layer = std::move(_layers.front());
        _layers.pop_front();
    }

    layer->detach();

    return layer;
}

std::unique_ptr<Layer> LayerManager::pop_back()
{
    if (!_sep)
        throw std::runtime_error("Attempted to pop an empty list head");

    std::unique_ptr<Layer> layer;

    if (*_sep == _layers.begin())
    {
        layer = std::move(**_sep);
        _layers.pop_front();
        _sep.reset();
    }
    else
    {
        auto temp = std::prev(*_sep);
        layer = std::move(**_sep);
        _layers.erase(*_sep);
        _sep.emplace(temp);
    }

    layer->detach();

    return layer;
}

std::unique_ptr<Layer> LayerManager::popGui_back()
{
    if (_layers.empty() || (_sep && *_sep == std::prev(_layers.end())))
        throw std::runtime_error("Attempted to pop an empty list tail");

    auto layer = std::move(_layers.back());
    _layers.pop_back();

    layer->detach();

    return layer;
}

} // namespace Runic
