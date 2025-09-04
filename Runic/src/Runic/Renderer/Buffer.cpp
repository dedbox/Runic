#include "Buffer.hpp"

namespace Runic
{
Buffer::Buffer(GraphicsContext* context, uint32_t id)
    : _context(context)
    , _id(id)
{
}

Buffer::~Buffer()
{
    if (_id != 0)
    {
        _context->destroyBuffer(_id);
        _id = 0;
    }
}

Buffer::Buffer(Buffer&& other) noexcept
    : _context(std::exchange(other._context, nullptr))
    , _id(std::exchange(other._id, 0))
{
}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
    if (this != &other)
    {
        _context->destroyBuffer(_id);
        _context = std::exchange(other._context, nullptr);
        _id      = std::exchange(other._id, 0);
    }
    return *this;
}

// Vertex Buffer ---------------------------------------------------------------

VertexBuffer::VertexBuffer(GraphicsContext* context, RendererId id, size_t count, size_t size)
    : Buffer(context, id)
    , _count(count)
    , _size(size)
{
}

void VertexBuffer::bind() const
{
    _context->bindVertexBuffer(_id);
}

void VertexBuffer::unbind() const
{
    _context->unbindVertexBuffer();
}

// Index Buffer ----------------------------------------------------------------

IndexBuffer::IndexBuffer(
    GraphicsContext* context, uint32_t id, size_t count, size_t size, IndexType type)
    : Buffer(context, id)
    , _count(count)
    , _size(size)
    , _type(type)
{
}

void IndexBuffer::bind() const
{
    _context->bindIndexBuffer(_id);
}

void IndexBuffer::unbind() const
{
    _context->unbindIndexBuffer();
}

} // namespace Runic
