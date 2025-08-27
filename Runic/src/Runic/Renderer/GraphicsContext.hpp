#pragma once

#include "glad/gl.h"

#include "Runic/Core/Window.hpp"

namespace Runic
{

using RendererId = uint32_t;

enum class BufferUsage : uint16_t
{
    Transient = GL_STREAM_DRAW,
    Static    = GL_STATIC_DRAW,
    Dynamic   = GL_DYNAMIC_DRAW,
};

static GLenum BufferUsageToGLenum(BufferUsage usage)
{
    return static_cast<GLenum>(usage);
}

enum class IndexType : uint16_t
{
    Byte  = GL_UNSIGNED_BYTE,
    Short = GL_UNSIGNED_SHORT,
    Int   = GL_UNSIGNED_INT,
};

static GLenum IndexTypeToGLenum(IndexType type)
{
    return static_cast<GLenum>(type);
}

enum class ElementType : uint8_t
{
    // clang-format off
    None = 0,
    Bool,
    Int, Int2, Int3, Int4,
    Float, Float2, Float3, Float4,
    Mat3, Mat4,
    // clang-format on
};

static GLenum ElementTypeToGLenum(ElementType type)
{
    switch (type)
    {
    case ElementType::None: return GL_NONE;
    case ElementType::Bool: return GL_BOOL;
    case ElementType::Int:
    case ElementType::Int2:
    case ElementType::Int3:
    case ElementType::Int4: return GL_INT;
    case ElementType::Float:
    case ElementType::Float2:
    case ElementType::Float3:
    case ElementType::Float4:
    case ElementType::Mat3:
    case ElementType::Mat4: return GL_FLOAT;
    }
    Core::Assert(false, "unknown element type");
    return 0;
}

class GraphicsContext
{
public:
    explicit GraphicsContext(const Window& window);

    void init();

    Window& getWindow() { return _window; }

    // Vertex Array

    [[nodiscard]] RendererId createVertexArray() const;
    void deleteVertexArray(RendererId id) const;

    void bindVertexArray(RendererId id) const;
    void unbindVertexArray() const;

    // Vertex Attribute

    void enableVertexAttribute(uint32_t index) const;
    void defineVertexAttributeData(
        uint32_t index,
        size_t count,
        ElementType type,
        bool normalized,
        size_t stride,
        const void* offset) const;

    // Vertex Buffer

    RendererId createVertexBuffer(const void* data, size_t size, BufferUsage usage) const;

    void bindVertexBuffer(RendererId id) const;
    void unbindVertexBuffer() const;

    // Index Buffer

    RendererId
    createIndexBuffer(const void* data, size_t count, IndexType type, BufferUsage usage) const;

    void bindIndexBuffer(RendererId id) const;
    void unbindIndexBuffer() const;

    // (both)

    void deleteBuffer(RendererId id) const;

private:
    Window _window;
    SDL_GLContext _native{nullptr};
};

} // namespace Runic
