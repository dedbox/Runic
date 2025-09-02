#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/Texture.hpp"

namespace Runic
{

using TexKey = std::pair<std::string, TextureSampling>;

class TextureManager
{
public:
    static void Load(
        GraphicsContext* context, const std::string& path, const TextureSampling& sampling);

    static std::shared_ptr<Texture> Find(
        GraphicsContext* context, const std::string& path, const TextureSampling& sampling);

private:
    static std::map<TexKey, std::shared_ptr<Texture>> _textures;
};

} // namespace Runic
