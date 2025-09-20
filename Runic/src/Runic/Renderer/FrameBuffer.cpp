#include "Runic/Renderer/FrameBuffer.hpp"

namespace Runic
{

FrameBuffer::FrameBuffer(GraphicsContext* context, RendererId id, std::shared_ptr<Texture> texture)
    : _context(context)
    , _id(id)
    , _texture(std::move(texture))
{
}

std::unique_ptr<FrameBuffer> FrameBuffer::Create(GraphicsContext* context, int width, int height)
{
    RendererId id = context->createFrameBuffer();
    auto texture  = Texture::Create(context, width, height);
    texture->unbind();
    context->setFrameBufferTexture(texture->getId());

    return std::unique_ptr<FrameBuffer>(new FrameBuffer(context, id, texture));
}

FrameBuffer::~FrameBuffer()
{
    _context->destroyFrameBuffer(_id);
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
    : _context(std::exchange(other._context, nullptr))
    , _id(std::exchange(other._id, 0))
    , _texture(std::move(other._texture))
    , _renderBuffer(std::move(other._renderBuffer))
{
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
{
    if (this != &other)
    {
        _context->destroyFrameBuffer(_id);
        _context      = std::exchange(other._context, nullptr);
        _id           = std::exchange(other._id, 0);
        _texture      = std::move(other._texture);
        _renderBuffer = std::move(other._renderBuffer);
    }
    return *this;
}

void FrameBuffer::bind() const
{
    _context->bindFrameBuffer(_id);
}

void FrameBuffer::unbind() const
{
    _context->unbindFrameBuffer();
}

void FrameBuffer::setRenderBuffer(std::unique_ptr<RenderBuffer> renderBuffer)
{
    _context->bindFrameBuffer(_id);
    _context->setFrameBufferRenderBuffer(renderBuffer->getId());

    if (!_context->isFrameBufferComplete())
        Core::Error("Frame buffer is not complete!");

    _context->unbindFrameBuffer();

    _renderBuffer = std::move(renderBuffer);
}

} // namespace Runic
