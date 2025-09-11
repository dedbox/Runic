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
    using namespace std::ranges;

    auto position = positions | views::chunk(3);
    auto normal   = normals | views::chunk(3);
    auto texCoord = texCoords | views::chunk(2);

    auto [positions_index, normal_index, texCoord_index] = CreateIndex();

    auto vertices = views::zip(positions_index, normal_index, texCoord_index) |
                    views::transform([&](auto args) {
                        auto&& [position_i, normal_i, texCoord_i] = args;

                        std::array<float, 8> result; // NOLINT
                        copy(position[position_i], result.begin());
                        copy(normal[normal_i], result.begin() + 3);
                        copy(texCoord[texCoord_i], result.begin() + 6);

                        return result;
                    }) |
                    views::join | to<std::vector<float>>();

    return VertexData<CubeLayout>(vertices);
}

std::tuple<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<uint32_t>> Cube::CreateIndex()
{
    using namespace std::ranges;

    auto position_index = face_positions | views::chunk(4) | views::transform([](auto chunk) {
                              std::array<uint32_t, 6> result; // NOLINT
                              result[0] = chunk[0];
                              result[1] = chunk[1];
                              result[2] = chunk[2];
                              result[3] = chunk[2];
                              result[4] = chunk[3];
                              result[5] = chunk[0];
                              return result;
                          }) |
                          views::join | to<std::vector<uint32_t>>();

    auto normal_index = std::ranges::views::iota(0) | std::ranges::views::take(36) |
                        std::ranges::views::transform([](uint32_t i) { return (i / 6) % 6; }) |
                        std::ranges::to<std::vector<uint32_t>>();

    auto texCoord_index = std::ranges::views::iota(0) | std::ranges::views::take(36) |
                          std::ranges::views::transform([](uint32_t i) { return i % 6; }) |
                          std::ranges::to<std::vector<uint32_t>>();

    return std::make_tuple(position_index, normal_index, texCoord_index);
}

} // namespace Runic::Graphics
