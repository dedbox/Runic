#include "Buffer.hpp"

namespace Runic
{
Buffer::Buffer(GraphicsContext* gc, uint32_t id)
    : _gc(gc)
    , _id(id)
{
}

Buffer::~Buffer()
{
    if (_id != 0)
    {
        _gc->destroyBuffer(_id);
        _id = 0;
    }
}

Buffer::Buffer(Buffer&& other) noexcept
    : _gc(std::exchange(other._gc, nullptr))
    , _id(std::exchange(other._id, 0))
{
}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
    if (this != &other)
    {
        _gc->destroyBuffer(_id);
        _gc = std::exchange(other._gc, nullptr);
        _id = std::exchange(other._id, 0);
    }
    return *this;
}

// Vertex Buffer ---------------------------------------------------------------

VertexBuffer::VertexBuffer(GraphicsContext* gc, RendererId id, size_t size)
    : Buffer(gc, id)
    , _size(size)
{
}

void VertexBuffer::bind() const
{
    _gc->bindVertexBuffer(_id);
}

void VertexBuffer::unbind() const
{
    _gc->unbindVertexBuffer();
}

// Index Buffer ----------------------------------------------------------------

IndexBuffer::IndexBuffer(
    GraphicsContext* gc, uint32_t id, size_t count, IndexType type, IndexMode mode)
    : Buffer(gc, id)
    , _count(count)
    , _type(type)
    , _mode(mode)
{
}

void IndexBuffer::bind() const
{
    _gc->bindIndexBuffer(_id);
}

void IndexBuffer::unbind() const
{
    _gc->unbindIndexBuffer();
}

} // namespace Runic
