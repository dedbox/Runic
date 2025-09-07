#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"
#include "Runic/Renderer/Texture.hpp"
#include "Runic/Renderer/VertexArray.hpp"
#include "Runic/Renderer/VertexData.hpp"

namespace Runic
{

class Mesh
{
public:
    static std::unique_ptr<Mesh> Create(GraphicsContext* context, DrawMode mode);

    template <typename Layout>
    void addVertices(
        const VertexData<Layout>& vertices,
        const std::vector<VertexAttribute>& layout,
        BufferUsage usage)
    {
        _vertexArray->addVertexBuffer(VertexBuffer::Create(_context, vertices, usage), layout);
    }

    void setIndices(const std::vector<uint32_t>& indices, IndexType type, BufferUsage usage);

    void addTexture(std::shared_ptr<Texture> texture);

    void draw(const ShaderProgram& shaderProgram) const;

private:
    GraphicsContext* _context;
    std::unique_ptr<VertexArray> _vertexArray;
    std::vector<std::shared_ptr<Texture>> _textures;

    // hide constructor
    explicit Mesh(GraphicsContext* context, DrawMode mode);
};

} // namespace Runic
