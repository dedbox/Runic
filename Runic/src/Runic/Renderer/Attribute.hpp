#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{

// Vertex Attribute --------------------------------------------------------------------------------

/** Constrains an Attribute component count according to its type. */
template <typename T, size_t N>
concept ValidAttributeParams = (
    // clang-format off
    (std::same_as<T, bool> && N == 1) ||
    (std::same_as<T, int>  && N >= 1 && N <= 4) ||
    (std::same_as<T, float> && (N >= 1 && N <= 4) || N == 9 || N == 16)
    // clang-format on
);

/** A purely compile-time vertex attribute abstraction
 *
 * Attributes are essentially phantom type tags for vertex data sources. These tags enable automatic
 * generation of vertex buffer layout code, and provide a means for ensuring Mesh and ShaderProgram
 * objects use the same vertex buffer formats at compile time.
 *
 * Each Attribute declares a type and number of individual components; for example, a vertex
 * consisting of a three-dimensional position and a pair of UV texture coordinates would use two
 * Attribute tags. Assuming all coordinates are normalized floating-point values, they are:
 *
 * 1. Attribute<float, 3> (a three-dimensional position)
 * 2. Attribute<float, 2> (a pair of UV texture coordinates)
 */
template <typename T, size_t N, bool B = false>
    requires(ValidAttributeParams<T, N>)
struct Attribute
{
    using type = T; /**< The underlying type */

    static constexpr size_t count   = N; /**< The number of components of type T */
    static constexpr bool normalize = B; /**< Whether the components need to be normalized */
    static constexpr size_t size    = N * sizeof(T); /**< The size in bytes of all components */
};

/** Ensures the template parameters of an Attribute instance are properly constrained. */
template <typename Attr>
concept ValidAttribute = (ValidAttributeParams<typename Attr::type, Attr::count>);

// Vertex Attribute Type ---------------------------------------------------------------------------

/** Converts an Attribute to an AttributeType. */
template <typename Attr>
constexpr AttributeType to_AttributeType()
{
    if constexpr (std::same_as<typename Attr::type, bool>)
        return AttributeType::Bool;

    else if constexpr (std::same_as<typename Attr::type, int>)
    {
        // clang-format off
        switch (Attr::count)
        {
        case 1: return AttributeType::Int;
        case 2: return AttributeType::Int2;
        case 3: return AttributeType::Int3;
        case 4: return AttributeType::Int4;
        }
        // clang-format on
    }

    else if constexpr (std::same_as<typename Attr::type, float>)
    {
        // clang-format off
        switch (Attr::count)
        {
        case 1: return AttributeType::Float;
        case 2: return AttributeType::Float2;
        case 3: return AttributeType::Float3;
        case 4: return AttributeType::Float4;
        case 9: return AttributeType::Mat3;
        case 16: return AttributeType::Mat4;
        }
        // clang-format on
    }
}

// Vertex Attribute Layout -------------------------------------------------------------------------

/** Constrains a parameter pack to Attributes with a common underlying type. */
template <typename Attr, typename... Attrs>
concept SameAttrType = (std::same_as<typename Attr::type, typename Attrs::type> && ...);

/** Ensures all Attribute declarations are valid and have a common underlying type. */
template <typename Attr, typename... Attrs>
concept ValidLayoutAttributes = requires {
    requires ValidAttribute<Attr>;
    requires(ValidAttribute<Attrs> && ...);
    requires SameAttrType<Attr, Attrs...>;
};

/** A compile-time collection of Attribute tags */
template <typename Attr, typename... Attrs>
    requires ValidLayoutAttributes<Attr, Attrs...>
struct Layout
{
    /** The common underlying type */
    using type = Attr::type;

    /** The total number of components in all Attributes */
    static constexpr size_t component_count = Attr::count + (Attrs::count + ...);

    /** The number of Attributes */
    static constexpr size_t attr_count = 1 + sizeof...(Attrs);

    /** The size in bytes of all attributes */
    static constexpr size_t stride = (Attr::size + ... + Attrs::size);

    /** The component count of each Attribute */
    static constexpr std::array<size_t, attr_count> counts{Attr::count, Attrs::count...};

    /** The size in bytes of each Attribute */
    static constexpr std::array<size_t, attr_count> sizes{Attr::size, Attrs::size...};

    static constexpr std::array<AttributeType, attr_count> attributeTypes{
        to_AttributeType<Attr>(), to_AttributeType<Attrs>()...};

    static constexpr std::array<bool, attr_count> normalizes{Attr::normalize, Attrs::normalize...};

    /** The byte index of the beginning of each Attribute */
    static constexpr std::array<size_t, attr_count> offsets = []() {
        std::array<size_t, attr_count> result{};
        result[0]  = 0;
        int offset = 0;
        for (size_t i = 1; i < attr_count; i++)
        {
            offset += sizes[i - 1];
            result[i] = offset;
        }
        return result;
    }();
};

template <typename T>
struct ValidLayoutImpl
{
};

template <typename A, typename... As>
struct ValidLayoutImpl<Layout<A, As...>>
{
    static constexpr bool value = ValidLayoutAttributes<A, As...>;
};

/** Ensures the template parameters of an AttributeLayout instance are properly constrained. */
template <typename T>
concept ValidLayout = ValidLayoutImpl<T>::value;

} // namespace Runic
