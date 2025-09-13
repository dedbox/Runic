#pragma once

#include "glm/ext/vector_float3.hpp"

#include "Runic/Renderer/Color.hpp"

namespace Runic::Light
{

struct Point
{
    glm::vec3 position;

    color ambient  = Color::Gray2;
    color diffuse  = Color::Gray5;
    color specular = Color::White;

    float constant  = 1.0F;
    float linear    = 1.0F;
    float quadratic = 1.0F;
};

struct Directional
{
    glm::vec3 direction;

    color ambient  = Color::Gray2;
    color diffuse  = Color::Gray5;
    color specular = Color::White;
};

struct Spot
{
    glm::vec3 position;
    glm::vec3 direction;

    float cutoff;
    float outerCutoff;

    color ambient  = Color::Gray2;
    color diffuse  = Color::Gray5;
    color specular = Color::White;

    float constant  = 1.0F;
    float linear    = 1.0F;
    float quadratic = 1.0F;
};

} // namespace Runic::Light
