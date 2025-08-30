#pragma once

namespace Runic
{

template <typename Key, typename Value>
class AssocList
{
public:
    using Pair = std::pair<Key, std::unique_ptr<Value>>;

    const std::vector<Pair>& data() const { return _data; }

    [[nodiscard]] bool empty() const { return _data.empty(); }

    void add(const Key& key, std::unique_ptr<Value> value)
    {
        auto it =
            std::lower_bound(_data.begin(), _data.end(), key, [](const Pair& p, const Key& k) {
                return p.first < k;
            });

        if (it != _data.end() && it->first == key)
            it->second = std::move(value);
        else
            _data.insert(it, {key, std::move(value)});
    }

    template <typename... Args>
    void emplace(const Key& key, Args&&... args)
    {
        auto it =
            std::lower_bound(_data.begin(), _data.end(), key, [](const Pair& p, const Key& k) {
                return p.first < k;
            });

        if (it != _data.end() && it->first == key)
            it->second = std::make_unique<Value>(std::forward<Args>(args)...);
        else
            _data.emplace(
                it,
                std::piecewise_construct,
                std::forward_as_tuple(key),
                std::forward_as_tuple(std::make_unique<Value>(std::forward<Args>(args)...)));
    }

    void remove(const Key& key)
    {
        auto it =
            std::lower_bound(_data.begin(), _data.end(), key, [](const Pair& a, const Key& k) {
                return a.first < k;
            });

        if (it != _data.end() && it->first == key)
            _data.erase(it);
    }

    void remove_all(const std::vector<Key>& keys)
    {
        std::unordered_set<Key> key_set = keys | std::ranges::to<std::unordered_set>();
        std::erase_if(_data, [&](const auto& pair) { return key_set.contains(pair.first); });
    }

    std::optional<std::reference_wrapper<Value>> find(const Key& key)
    {
        auto it =
            std::lower_bound(_data.begin(), _data.end(), key, [](const Pair& p, const Key& k) {
                return p.first < k;
            });

        if (it != _data.end() && it->first == key)
        {
            return *it->second.get();
        }
        else
            return std::nullopt;
    }

    std::vector<Pair> data() { return _data; }

    auto values()
    {
        return _data |
               std::ranges::views::transform([](auto& p) -> Value& { return *p.second.get(); });
    }

    auto pairs()
    {
        return _data |
               std::ranges::views::transform([](auto& pair) -> std::pair<const Key&, Value&> {
                   return {pair.first, *pair.second.get()};
               });
    }

private:
    std::vector<Pair> _data;
};

} // namespace Runic
