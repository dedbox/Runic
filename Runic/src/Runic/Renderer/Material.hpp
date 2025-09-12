#pragma once

#include "Runic/Renderer/Color.hpp"
#include "Runic/Renderer/Texture.hpp"

namespace Runic
{

struct PhongMaterial
{
    color ambient;
    color diffuse;
    color specular;
    float shininess;
};

struct LightingMapMaterial
{
    TextureId diffuse;
    TextureId specular;
    float shininess;
};

using Material = std::variant<PhongMaterial, LightingMapMaterial>;

} // namespace Runic
