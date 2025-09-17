#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/RenderObject.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"

namespace Runic::Graphics
{

class Plane : public RenderObject
{
private:
    explicit Plane(GraphicsContext* context)
        : RenderObject(context)
    {
    }

public:
    static std::unique_ptr<Plane> Create(GraphicsContext* context);

private:
    static constexpr std::vector<float> CreateData();
    static constexpr std::vector<uint32_t> CreateIndex();

    static constexpr std::array<float, 18> Positions();
    static constexpr std::array<float, 3> Normals();
    static constexpr std::array<float, 8> TexCoords();
};

} // namespace Runic::Graphics
