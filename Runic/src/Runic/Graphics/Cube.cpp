#include "Runic/Graphics/Cube.hpp"

namespace Runic::Graphics
{

std::unique_ptr<Cube> Cube::Create(Runic::GraphicsContext* context)
{
    auto cube = std::unique_ptr<Cube>(new Cube(context));

    cube->createMesh(DrawMode::Triangles);

    auto vertexBuffer = VertexBuffer::Create(context, CreateData(), BufferUsage::Static);
    std::vector<VertexAttribute> layout = {
        {.type = AttributeType::Float3, .normalize = false},
        {.type = AttributeType::Float3, .normalize = false},
        {.type = AttributeType::Float2, .normalize = false},
    };

    cube->mesh->addVertexBuffer(std::move(vertexBuffer), layout);
    cube->mesh->setIndices(CreateIndex(), IndexType::Int, BufferUsage::Static);

    return cube;
}

constexpr std::vector<float> Cube::CreateData()
{
    const auto position = Positions() | std::ranges::views::chunk(3);
    const auto normal   = Normals() | std::ranges::views::chunk(3);
    const auto texCoord = TexCoords() | std::ranges::views::chunk(2);

    const auto faces = Faces();

    std::vector<float> vertices(8L * 24L);
    for (long i = 0; i < 24; i++)
    {
        const long offset = 8L * i;
        std::ranges::copy(position[faces[i]], vertices.begin() + offset); // NOLINT
        std::ranges::copy(normal[i / 4], vertices.begin() + offset + 3L);
        std::ranges::copy(texCoord[i % 4], vertices.begin() + offset + 6L);
    }

    return vertices;
}

constexpr std::vector<uint32_t> Cube::CreateIndex()
{
    return std::ranges::views::iota(0, 24) | std::ranges::views::chunk(4) |
           std::ranges::views::transform([](const auto& chunk) {
               std::array<uint32_t, 6> result; // NOLINT
               result[0] = chunk[0];
               result[1] = chunk[1];
               result[2] = chunk[2];
               result[3] = chunk[2];
               result[4] = chunk[3];
               result[5] = chunk[0];
               return result;
           }) |
           std::ranges::views::join | std::ranges::to<std::vector<uint32_t>>();
}

constexpr std::array<uint32_t, 24> Cube::Faces()
{
    // clang-format off
    return {
        0, 1, 2, 3,             // back
        4, 5, 6, 7,             // front
        7, 3, 0, 4,             // left
        6, 2, 1, 5,             // right
        0, 1, 5, 4,             // bottom
        3, 2, 6, 7,             // top
    };
    // clang-format on    
}

constexpr std::array<float, 24> Cube::Positions()
{
    constexpr float x = 0.5F;
    // clang-format off
    return {
        -x, -x, -x,             // back bottom left
         x, -x, -x,             // back bottom right
         x,  x, -x,             // back top right
        -x,  x, -x,             // back top left

        -x, -x,  x,             // front bottom left
         x, -x,  x,             // front bottom right
         x,  x,  x,             // front top right
        -x,  x,  x,             // front top left
    };
    // clang-format on
}

constexpr std::array<float, 18> Cube::Normals()
{
    // clang-format off
    return {
         0,  0, -1,             // back
         0,  0,  1,             // front
        -1,  0,  0,             // left
         1,  0,  0,             // right
         0, -1,  0,             // bottom
         0,  1,  0,             // top
    };
    // clang-format on
}

constexpr std::array<float, 8> Cube::TexCoords()
{
    // clang-format off
    return {
        0, 0,                   // bottom left
        1, 0,                   // bottom right
        1, 1,                   // top right
        0, 1,                   // top left
    };
    // clang-format on
}

} // namespace Runic::Graphics
