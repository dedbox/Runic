#include "Runic/Renderer/RenderBuffer.hpp"

namespace Runic
{

RenderBuffer::RenderBuffer(GraphicsContext* context, RendererId id)
    : _context(context)
    , _id(id)
{
}

std::unique_ptr<RenderBuffer> RenderBuffer::Create(GraphicsContext* context, int width, int height)
{
    RendererId id = context->createRenderBuffer(width, height);

    return std::unique_ptr<RenderBuffer>(new RenderBuffer(context, id));
}

RenderBuffer::~RenderBuffer()
{
    _context->destroyRenderBuffer(_id);
}

RenderBuffer::RenderBuffer(RenderBuffer&& other) noexcept
    : _context(std::exchange(other._context, nullptr))
    , _id(std::exchange(other._id, 0))
{
}

RenderBuffer& RenderBuffer::operator=(RenderBuffer&& other) noexcept
{
    if (this != &other)
    {
        _context->destroyRenderBuffer(_id);
        _context = std::exchange(other._context, nullptr);
        _id      = std::exchange(other._id, 0);
    }
    return *this;
}

void RenderBuffer::bind() const
{
    _context->bindRenderBuffer(_id);
}

void RenderBuffer::unbind() const
{
    _context->unbindRenderBuffer();
}

} // namespace Runic
