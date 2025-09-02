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
    SDL_Surface* raw = IMG_Load(std::format("textures/{}", path).c_str());

    const std::string format = SDL_GetPixelFormatName(raw->format);

    Core::Info("Image loaded:");
    Core::Info("    format: {}", format);

    // SDL_Surface* converted = SDL_ConvertSurface(raw, SDL_PIXELFORMAT_RGBA8888);
    // if (!converted)
    //     throw SDLException("Could not convert surface format");

    TexKey key(path, sampling);

    _textures[key] = Texture::Create(context, raw, sampling);

    // SDL_DestroySurface(converted);
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
