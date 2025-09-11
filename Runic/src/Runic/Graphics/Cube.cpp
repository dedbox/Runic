#include "Runic/Graphics/Cube.hpp"
#include <algorithm>

namespace Runic::Graphics
{

std::unique_ptr<Cube> Cube::Create(
    Runic::GraphicsContext* context, std::shared_ptr<Runic::ShaderProgram<CubeLayout>>&& shader)
{
    auto cube = std::unique_ptr<Cube>(new Cube(context));

    cube->createMesh(DrawMode::Triangles);
    cube->mesh->setVertexBuffers(
        std::make_tuple(
            VertexBuffer<CubeLayout>::Create(context, CreateData(), BufferUsage::Static)));

    cube->shader = std::move(shader);

    return cube;
}

VertexData<CubeLayout> Cube::CreateData()
{
    auto position = Positions() | std::ranges::views::chunk(3);
    auto normal   = Normals() | std::ranges::views::chunk(3);
    auto texCoord = TexCoords() | std::ranges::views::chunk(2);
    auto vertices = std::ranges::views::zip(PositionIndex(), NormalIndex(), TexCoordIndex()) |
                    std::ranges::views::transform([&](auto args) {
                        auto&& [p, n, t] = args;
                        std::array<float, 8> result; // NOLINT
                        std::ranges::copy(position[p], result.begin());
                        std::ranges::copy(normal[n], result.begin() + 3);
                        std::ranges::copy(texCoord[t], result.begin() + 6);
                        return result;
                    }) |
                    std::ranges::views::join | std::ranges::to<std::vector<float>>();
    return VertexData<CubeLayout>(vertices);
}

constexpr std::array<float, 24> Cube::Positions()
{
    float l = 0.5F;
    // clang-format off
    return {
        -l, -l, -l,             // back bottom left
         l, -l, -l,             // back bottom right
         l,  l, -l,             // back top right
        -l,  l, -l,             // back top left

        -l, -l,  l,             // front bottom left
         l, -l,  l,             // front bottom right
         l,  l,  l,             // front top right
        -l,  l,  l,             // front top left
    };
    // clang-format on
}

constexpr std::array<float, 18> Cube::Normals()
{
    float O = 0.0F, l = 1.0F;
    // clang-format off
    return {
         O,  O, -l,             // back
         O,  O,  l,             // front
        -l,  O,  O,             // left
         l,  O,  O,             // right
         O, -l,  O,             // bottom
         O,  l,  O,             // top
    };
    // clang-format on
}

constexpr std::array<float, 12> Cube::TexCoords()
{
    float O = 0.0F, l = 1.0F;
    // clang-format off
    return {
        O, O,                   // bottom left
        l, O,                   // bottom right
        l, l,                   // top right

        l, l,                   // top right
        O, l,                   // top left
        O, O,                   // bottom left
    };
    // clang-format on
}

constexpr std::array<uint32_t, 36> Cube::PositionIndex()
{
    // clang-format off
    static constexpr std::array<uint32_t, 24> faces {
        0, 1, 2, 3,             // back
        4, 5, 6, 7,             // front
        7, 3, 0, 4,             // left
        6, 2, 1, 5,             // right
        0, 1, 5, 4,             // bottom
        3, 2, 6, 7,             // top
    };
    // clang-format on

    std::array<uint32_t, 36> positions; // NOLINT
    for (int i = 0; i < 6; i++)
    {
        const int source = 4 * i;
        const int dest   = 6 * i;
        // NOLINTBEGIN
        positions[dest + 0] = faces[source + 0];
        positions[dest + 1] = faces[source + 1];
        positions[dest + 2] = faces[source + 2];
        positions[dest + 3] = faces[source + 2];
        positions[dest + 4] = faces[source + 3];
        positions[dest + 5] = faces[source + 0];
        // NOLINTEND
    }

    return positions;
}

constexpr std::array<uint32_t, 36> Cube::NormalIndex()
{
    std::array<uint32_t, 36> index; // NOLINT
    for (int i = 0; i < 36; i++)
        index[i] = i / 6; // NOLINT
    return index;
}

constexpr std::array<uint32_t, 36> Cube::TexCoordIndex()
{
    std::array<uint32_t, 36> index; // NOLINT
    for (int i = 0; i < 36; i++)
        index[i] = i % 6; // NOLINT
    return index;
}

} // namespace Runic::Graphics
