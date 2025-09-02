#pragma once

#include "Runic/Renderer/Color.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{

struct TextureSampling
{
    TextureWrap wrapS          = TextureWrap::Repeat;
    TextureWrap wrapT          = TextureWrap::Repeat;
    color borderColor          = Color::Magenta;
    TextureMinFilter minFilter = TextureMinFilter::Linear;
    TextureMagFilter magFilter = TextureMagFilter::Linear;
    bool mipmap                = true;

    auto operator<=>(const TextureSampling&) const = default;
};

class Texture
{
public:
    static std::shared_ptr<Texture> Create(
        GraphicsContext* context, SDL_Surface* surface, const TextureSampling& sampling);

    ~Texture();

    // allow moving
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    // prevent copying
    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    void bind() const;
    void unbind() const;

private:
    GraphicsContext* _context;
    RendererId _id;

    // hide constrctor
    Texture(GraphicsContext* context, RendererId id);
};

} // namespace Runic
