#pragma once

namespace Runic
{

template <typename Key, typename Value>
class AssocList
{
public:
    using Pair = std::pair<Key, Value>;

    const std::vector<Pair>& data() const { return _data; }

    [[nodiscard]] bool empty() const { return _data.empty(); }

    void add(const Key& key, Value&& value)
    {
        const Pair& new_pair = {key, std::move(value)};

        auto it = std::lower_bound(
            _data.begin(), _data.end(), new_pair,
            [](const Pair& a, const Pair& b) { return a.first < b.first; });

        if (it != _data.end() && it->first == key)
            it->second = std::move(new_pair.second);
        else
            _data.insert(it, std::move(new_pair));
    }

    void remove(const Key& key)
    {
        auto it = std::lower_bound(
            _data.begin(), _data.end(), key,
            [](const Pair& a, const Key& k) { return a.first < k; });

        if (it != _data.end() && it->first == key) _data.erase(it);
    }

    std::optional<std::reference_wrapper<Value>> find(const Key& key)
    {
        auto it = std::lower_bound(
            _data.begin(), _data.end(), key,
            [](const Pair& p, const Key& k) { return p.first < k; });

        if (it != _data.end() && it->first == key)
            return std::ref(it->second);
        else
            return std::nullopt;
    }

private:
    std::vector<Pair> _data;
};

} // namespace Runic
