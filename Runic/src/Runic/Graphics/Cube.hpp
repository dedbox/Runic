#pragma once

#include "Runic/Renderer/Attribute.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/RenderObject.hpp"
#include "Runic/Renderer/VertexData.hpp"

namespace Runic::Graphics
{

using CubeLayout = Layout<
    Attribute<float, 3>,  // position
    Attribute<float, 3>,  // normal
    Attribute<float, 2>>; // texture coordinates

class Cube : public RenderObject<CubeLayout>
{
private:
    explicit Cube(Runic::GraphicsContext* context)
        : Runic::RenderObject<CubeLayout>(context)
    {
    }

public:
    static std::unique_ptr<Cube> Create(
        Runic::GraphicsContext* context,
        std::shared_ptr<Runic::ShaderProgram<CubeLayout>>&& shader);

private:
    static constexpr VertexData<CubeLayout> CreateData();
    static constexpr std::vector<uint32_t> CreateIndex();

    static constexpr std::array<uint32_t, 24> Faces();

    static constexpr std::array<float, 24> Positions();
    static constexpr std::array<float, 18> Normals();
    static constexpr std::array<float, 8> TexCoords();
};

} // namespace Runic::Graphics
