#pragma once

namespace Runic
{

// Vertex Attribute --------------------------------------------------------------------------------

template <typename T, size_t N, bool Norm = false>
struct VertexAttribute2
{
    using type = T;

    static constexpr size_t size    = N;
    static constexpr bool normalize = Norm;
};

template <typename Attr>
static constexpr size_t attribute_size()
{
    return (sizeof(typename Attr::type) * Attr::size);
}

template <typename... Attrs>
static constexpr size_t layout_count()
{
    return ((Attrs::type, 1) + ...);
}

template <typename... Attrs>
static constexpr size_t layout_size()
{
    return ((sizeof(typename Attrs::type) * Attrs::size) + ...);
}

template <typename Attr, typename... Attrs>
static constexpr bool valid_layout()
{
    return (std::is_same_v<typename Attr::type, typename Attrs::type> && ...);
}

// Vertex Vector -----------------------------------------------------------------------------------

template <typename Attr, typename... Attrs>
    requires(valid_layout<Attr, Attrs...>())
class VertexVector
{
public:
    using T = Attr::type;

    static constexpr size_t N = Attr::size;

    VertexVector(std::initializer_list<T> init_list)
        : _vertices(init_list)
    {
        Core::Assert(init_list.size() % N == 0, "Vector size is not a multiple of {}", N);
    }

    const T* data() const { return _vertices.data(); }

    const size_t count() const { return _vertices.size(); }

private:
    std::vector<T> _vertices;
};

} // namespace Runic
