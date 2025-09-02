#include "Runic/Renderer/Texture.hpp"

namespace Runic
{

std::shared_ptr<Texture> Texture::Create(
    GraphicsContext* context, SDL_Surface* surface, const TextureSampling& sampling)
{
    RendererId id = context->createTexture(surface);

    context->setTextureWrapS(sampling.wrapS);
    context->setTextureWrapT(sampling.wrapT);

    context->setTextureBorderColor(sampling.borderColor);

    context->setTextureMinFilter(sampling.minFilter);
    context->setTextureMagFilter(sampling.magFilter);

    if (sampling.mipmap)
        context->generateMipmap();

    return std::shared_ptr<Texture>(new Texture(context, id));
}

Texture::Texture(GraphicsContext* context, RendererId id)
    : _context(context)
    , _id(id)
{
}

Texture::~Texture()
{
    _context->destroyTexture(_id);
}

Texture::Texture(Texture&& other) noexcept
    : _context(std::exchange(other._context, nullptr))
    , _id(std::exchange(other._id, 0))
{
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        _context->destroyTexture(_id);
        _context = std::exchange(other._context, nullptr);
        _id      = std::exchange(other._id, 0);
    }
    return *this;
}

void Texture::bind() const
{
    _context->bindTexture(_id);
}

void Texture::unbind() const
{
    _context->unbindTexture();
}

} // namespace Runic
