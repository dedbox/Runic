#include "Runic/Renderer/TextureManager.hpp"

#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_surface.h"
#include "SDL3_image/SDL_image.h"

namespace Runic
{

std::map<TexKey, std::shared_ptr<Texture>> TextureManager::_textures;

void TextureManager::Load(
    GraphicsContext* context, const std::string& path, const TextureSampling& sampling)
{
    SDL_Surface* raw = IMG_Load(path.c_str());
    if (!raw)
        throw SDLException(std::format("Could not load image `texures/{}'", path));

    Core::Info("Image `{}' loaded", path);
    Core::Info("    format: {}", SDL_GetPixelFormatName(raw->format));

    if (!SDL_FlipSurface(raw, SDL_FLIP_VERTICAL))
        throw SDLException(std::format("Could not flip image `texures/{}'", path));

    SDL_Surface* converted = SDL_ConvertSurface(raw, SDL_PIXELFORMAT_RGBA32);
    if (!converted)
        throw SDLException(
            std::format("Could not convert image `texures/{}' format to RGBA32", path));

    TexKey key(path, sampling);

    _textures[key] = Texture::Create(context, converted, sampling);

    SDL_DestroySurface(converted);
    SDL_DestroySurface(raw);
}

std::shared_ptr<Texture> TextureManager::Find(
    GraphicsContext* context, const std::string& path, const TextureSampling& sampling)
{
    const TexKey key(path, sampling);

    if (!_textures.contains(key))
        Load(context, path, sampling);

    return _textures[key];
}

} // namespace Runic
