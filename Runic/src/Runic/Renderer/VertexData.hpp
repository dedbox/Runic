#pragma once

#include "Runic/Renderer/Attribute.hpp"

namespace Runic
{

template <typename Layout>
    requires(ValidLayout<Layout>)
class VertexData
{
public:
    using Type = Layout::type;

    explicit VertexData(const std::vector<typename Layout::type>& data)
        : _data(data)
    {
    }

    const Type* data() const { return _data.data(); }
    const size_t count() const { return _data.size(); }
    const size_t size() const { return count() * sizeof(Type); }

private:
    std::vector<typename Layout::type> _data;
};

} // namespace Runic
