#pragma once

#include "glm/ext/vector_float4.hpp"

namespace Runic
{

using color = glm::vec4;

static auto operator<=>(const color& lhs, const color& rhs)
{
    // clang-format off
    if (auto cmp = lhs.r <=> rhs.r; cmp != 0) return cmp;
    if (auto cmp = lhs.g <=> rhs.g; cmp != 0) return cmp;
    if (auto cmp = lhs.b <=> rhs.b; cmp != 0) return cmp;
    return lhs.a <=> rhs.a;
    // clang-format on
}

namespace Color
{

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

static constexpr color Black = Color::hex(0x000000FF);
static constexpr color White = Color::hex(0xFFFFFFFF);

static constexpr color Red   = Color::hex(0xFF0000FF);
static constexpr color Green = Color::hex(0x00FF00FF);
static constexpr color Blue  = Color::hex(0x0000FFFF);

static constexpr color Cyan    = Color::hex(0x00FFFFFF);
static constexpr color Magenta = Color::hex(0xFF00FFFF);
static constexpr color Yellow  = Color::hex(0xFFFF00FF);

static constexpr color Gray1 = color(0.1F, 0.1F, 0.1F, 1.0F);
static constexpr color Gray2 = color(0.2F, 0.2F, 0.2F, 1.0F);
static constexpr color Gray3 = color(0.3F, 0.3F, 0.3F, 1.0F);
static constexpr color Gray4 = color(0.4F, 0.4F, 0.4F, 1.0F);
static constexpr color Gray5 = color(0.5F, 0.5F, 0.5F, 1.0F);
static constexpr color Gray6 = color(0.6F, 0.6F, 0.6F, 1.0F);
static constexpr color Gray7 = color(0.7F, 0.7F, 0.7F, 1.0F);
static constexpr color Gray8 = color(0.8F, 0.8F, 0.8F, 1.0F);
static constexpr color Gray9 = color(0.9F, 0.9F, 0.9F, 1.0F);

} // namespace Color
} // namespace Runic
