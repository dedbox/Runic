#pragma once

#include "Runic/Renderer/CubeMap.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/Texture.hpp"

namespace Runic
{

using TexKey = std::pair<std::string, TextureSampling>;

class TextureManager
{
public:
    static void Load(
        GraphicsContext* context, const std::string& path, const TextureSampling& sampling = {});

    static std::shared_ptr<Texture> Find(
        GraphicsContext* context, const std::string& path, const TextureSampling& sampling = {});

    static void LoadCubeMap(GraphicsContext* context, std::array<std::string, 6> paths);

    static std::shared_ptr<CubeMap> FindCubeMap(
        GraphicsContext* context, const std::array<std::string, 6>& paths);

private:
    static std::map<TexKey, std::shared_ptr<Texture>> _textures;
    static std::map<std::array<std::string, 6>, std::shared_ptr<CubeMap>> _cubeMaps;
};

} // namespace Runic
