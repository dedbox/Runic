#pragma once

namespace Runic
{
class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void bind() const = 0;

    virtual void unbind() const = 0;

    static VertexBuffer* create(const float* vertices, uint32_t size);
};

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void bind() const = 0;

    virtual void unbind() const = 0;

    virtual uint32_t getCount() const = 0;

    static IndexBuffer* create(const uint32_t* indices, uint32_t count);
};
}
