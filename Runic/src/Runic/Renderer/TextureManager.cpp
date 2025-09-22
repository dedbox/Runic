#include "Runic/Renderer/TextureManager.hpp"

#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_surface.h"
#include "SDL3_image/SDL_image.h"

namespace Runic
{

std::map<TexKey, std::shared_ptr<Texture>> TextureManager::_textures;
std::map<std::array<std::string, 6>, std::shared_ptr<CubeMap>> TextureManager::_cubeMaps;

void TextureManager::Load(
    GraphicsContext* context, const std::string& path, const TextureSampling& sampling)
{
    SDL_Surface* raw = IMG_Load(path.c_str());
    if (!raw)
        throw SDLException(std::format("Could not load image `{}'", path));

    Core::Info("Image `{}' loaded", path);
    Core::Info("    format: {}", SDL_GetPixelFormatName(raw->format));

    if (!SDL_FlipSurface(raw, SDL_FLIP_VERTICAL))
        throw SDLException(std::format("Could not flip image `{}'", path));

    SDL_Surface* converted = SDL_ConvertSurface(raw, SDL_PIXELFORMAT_RGBA32);
    if (!converted)
        throw SDLException(std::format("Could not convert image `{}' format to RGBA32", path));

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

void TextureManager::LoadCubeMap(GraphicsContext* context, std::array<std::string, 6> paths)
{
    auto surfaces =
        paths | std::ranges::views::transform([&](const auto& path) {
            SDL_Surface* raw = IMG_Load(path.c_str());
            if (!raw)
                throw SDLException(std::format("Could not load image `{}'", path));
            Core::Info("Image `{}' loaded", path);
            Core::Info("    format: {}", SDL_GetPixelFormatName(raw->format));

            SDL_Surface* converted = SDL_ConvertSurface(raw, SDL_PIXELFORMAT_RGBA32);
            if (!converted)
                throw SDLException(
                    std::format("Could not convert images `{}' format to RGBA32", path));

            SDL_DestroySurface(raw);

            return converted;
        }) |
        std::ranges::to<std::vector>();

    _cubeMaps[paths] = CubeMap::Create(context, surfaces);

    for (const auto& surface : surfaces)
        SDL_DestroySurface(surface);
}

std::shared_ptr<CubeMap> TextureManager::FindCubeMap(
    GraphicsContext* context, const std::array<std::string, 6>& paths)
{
    if (!_cubeMaps.contains(paths))
        LoadCubeMap(context, paths);

    return _cubeMaps[paths];
}

} // namespace Runic
