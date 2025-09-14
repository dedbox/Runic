#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/RenderObject.hpp"

namespace Runic::Graphics
{

class Cube : public RenderObject
{
private:
    explicit Cube(Runic::GraphicsContext* context)
        : Runic::RenderObject(context)
    {
    }

public:
    static std::unique_ptr<Cube> Create(
        Runic::GraphicsContext* context, std::shared_ptr<Runic::ShaderProgram>&& shader);

private:
    static constexpr std::vector<float> CreateData();
    static constexpr std::vector<uint32_t> CreateIndex();

    static constexpr std::array<uint32_t, 24> Faces();

    static constexpr std::array<float, 24> Positions();
    static constexpr std::array<float, 18> Normals();
    static constexpr std::array<float, 8> TexCoords();
};

} // namespace Runic::Graphics
