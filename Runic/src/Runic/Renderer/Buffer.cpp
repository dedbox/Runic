#include "Buffer.hpp"

namespace Runic
{
Buffer::Buffer(GraphicsContext* gc, uint32_t id)
    : _gc(gc)
    , _id(id)
{
}

Buffer::Buffer(Buffer&& other) noexcept
    : _gc(other._gc)
    , _id(other._id)
{
    other._id = 0;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
    if (this != &other)
    {
        if (_id != 0)
        {
            Core::Assert(_id != other._id, "duplicated buffer id");
            _gc->deleteBuffer(_id);
        }
        _id       = other._id;
        other._id = 0;
    }

    return *this;
}

Buffer::~Buffer()
{
    if (_id != 0)
    {
        _gc->deleteBuffer(_id);
        _id = 0;
    }
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

IndexBuffer::IndexBuffer(GraphicsContext* gc, uint32_t id, size_t count, IndexType type)
    : Buffer(gc, id)
    , _count(count)
    , _type(type)
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
