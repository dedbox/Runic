#include "Runic/Graphics/Plane.hpp"

#include "Runic/Renderer/Buffer.hpp"

namespace Runic::Graphics
{

std::unique_ptr<Plane> Plane::Create(GraphicsContext* context)
{
    auto plane = std::unique_ptr<Plane>(new Plane(context));

    plane->createMesh(DrawMode::Triangles);

    auto vertexBuffer = VertexBuffer::Create(context, CreateData(), BufferUsage::Static);
    std::vector<VertexAttribute> layout = {
        {.type = AttributeType::Float3, .normalize = false},
        {.type = AttributeType::Float3, .normalize = false},
        {.type = AttributeType::Float2, .normalize = false},
    };

    plane->mesh->addVertexBuffer(std::move(vertexBuffer), layout);
    plane->mesh->setIndices(CreateIndex(), IndexType::Int, BufferUsage::Static);

    return plane;
}

constexpr std::vector<float> Plane::CreateData()
{
    const auto position = Positions() | std::ranges::views::chunk(3);
    const auto normal   = Normals();
    const auto texCoord = TexCoords() | std::ranges::views::chunk(2);

    std::vector<float> vertices(8L * 4L);
    for (long i = 0; i < 4; i++)
    {
        const long offset = 8L * i;
        std::ranges::copy(position[i], vertices.begin() + offset);
        std::ranges::copy(normal, vertices.begin() + offset + 3L);
        std::ranges::copy(texCoord[i], vertices.begin() + offset + 6L);
    }

    return vertices;
}

constexpr std::vector<uint32_t> Plane::CreateIndex()
{
    return {0, 1, 2, 0, 2, 3};
}

constexpr std::array<float, 18> Plane::Positions()
{
    constexpr float x = 0.5F;
    // clang-format off
    return {
         x, 0,  x,              // front right
        -x, 0,  x,              // front left
        -x, 0, -x,              // back left
         x, 0, -x,              // back right
    };
    // clang-format on
}

constexpr std::array<float, 3> Plane::Normals()
{
    // clang-format off
    return {
        0, 1, 0,
    };
    // clang-format on
}

constexpr std::array<float, 8> Plane::TexCoords()
{
    // clang-format off
    return {
        2, 0,                   // front right
        0, 0,                   // front left
        0, 2,                   // back left
        2, 2,                   // back right
    };
    // clang-format on    
}

} // namespace Runic::Graphics
