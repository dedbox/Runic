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

std::unique_ptr<VertexBuffer> VertexBuffer::Create(
    GraphicsContext* context, const void* data, size_t size, BufferUsage usage)
{
    RendererId id = context->createVertexBuffer(data, size, usage);
    return std::unique_ptr<VertexBuffer>(new VertexBuffer(context, id, size));
}

VertexBuffer::VertexBuffer(GraphicsContext* context, RendererId id, size_t size)
    : Buffer(context, id)
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

std::unique_ptr<IndexBuffer> IndexBuffer::Create(
    GraphicsContext* context,
    const void* data,
    size_t count,
    IndexMode mode,
    IndexType type,
    BufferUsage usage)
{
    RendererId id = context->createIndexBuffer(data, count, type, usage);
    return std::unique_ptr<IndexBuffer>(new IndexBuffer(context, id, count, type, mode));
}

IndexBuffer::IndexBuffer(
    GraphicsContext* context, uint32_t id, size_t count, IndexType type, IndexMode mode)
    : Buffer(context, id)
    , _count(count)
    , _type(type)
    , _mode(mode)
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
