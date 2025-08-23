#pragma once

#include "glm/ext/vector_float4.hpp"
#include <cstdint>

namespace Runic
{

using color = glm::vec4;

namespace Color
{

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
constexpr uint8_t hex_r(uint32_t value)
{
    return (value >> 24U) & 0xFFU;
}

constexpr uint8_t hex_g(uint32_t value)
{
    return (value >> 16U) & 0xFFU;
}

constexpr uint8_t hex_b(uint32_t value)
{
    return (value >> 8U) & 0xFFU;
}

constexpr uint8_t hex_a(uint32_t value)
{
    return value & 0xFFU;
}

constexpr inline color hex(uint32_t value)
{
    return {
        static_cast<float>(hex_r(value)) / 255.0F,
        static_cast<float>(hex_g(value)) / 255.0F,
        static_cast<float>(hex_b(value)) / 255.0F,
        static_cast<float>(hex_a(value)) / 255.0F,
    };
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

static constexpr color Black = Color::hex(0x000000FF);
static constexpr color White = Color::hex(0xFFFFFFFF);

static constexpr color Red   = Color::hex(0xFF0000FF);
static constexpr color Green = Color::hex(0x00FF00FF);
static constexpr color Blue  = Color::hex(0x0000FFFF);

static constexpr color Cyan    = Color::hex(0x00FFFFFF);
static constexpr color Magenta = Color::hex(0xFF00FFFF);
static constexpr color Yellow  = Color::hex(0xFFFF00FF);

} // namespace Color
} // namespace Runic
