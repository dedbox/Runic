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

    static VertexData<CubeLayout> CreateData();

    static std::tuple<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<uint32_t>>
    CreateIndex();

private:
    // clang-format off
    static constexpr std::array<float, 24> positions = {
        -0.5F, -0.5F, -0.5F,    // 0
         0.5F, -0.5F, -0.5F,    // 1
         0.5F,  0.5F, -0.5F,    // 2
        -0.5F,  0.5F, -0.5F,    // 3
        -0.5F, -0.5F,  0.5F,    // 4
         0.5F, -0.5F,  0.5F,    // 5
         0.5F,  0.5F,  0.5F,    // 6
        -0.5F,  0.5F,  0.5F,    // 7
    };

    static constexpr std::array<float, 18> normals = {
         0.0F,  0.0F, -1.0F,
         0.0F,  0.0F,  1.0F,
        -1.0F,  0.0F,  0.0F,
         1.0F,  1.0F,  0.0F,
         0.0F, -1.0F,  0.0F,
         0.0F,  1.0F,  0.0F,
    };

    static constexpr std::array<float, 16> texCoords = {
        0.0F, 0.0F,
        1.0F, 0.0F,
        1.0F, 1.0F,
        1.0F, 1.0F,
        0.0F, 1.0F,
        0.0F, 0.0F,
    };

    static constexpr std::array<uint32_t, 24> face_positions = {
        0, 1, 2, 3,
        4, 5, 6, 7,
        7, 3, 0, 4,
        6, 2, 1, 5,
        0, 1, 5, 4,
        3, 2, 6, 7,
    };
};

} // namespace Runic::Graphics
