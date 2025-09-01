#include "Mesh.hpp"
#include "Runic/Renderer/Buffer.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/VertexArray.hpp"

namespace Runic
{

Mesh::Mesh(GraphicsContext* context)
    : _context(context)
{
    RendererId id = _context->createVertexArray();
    _vertexArray  = std::make_unique<VertexArray>(_context, id);
}

void Mesh::addVertices(
    const std::vector<float>& vertices,
    const std::vector<VertexAttribute>& layout,
    BufferUsage usage)
{
    size_t size   = vertices.size() * sizeof(float);
    RendererId id = _context->createVertexBuffer(vertices.data(), size, usage);
    _vertexArray->addVertexBuffer(std::make_unique<VertexBuffer>(_context, id, size), layout);
}

void Mesh::setIndices(
    const std::vector<uint32_t>& indices, IndexMode mode, IndexType type, BufferUsage usage)
{
    RendererId id = _context->createIndexBuffer(indices.data(), indices.size(), type, usage);
    _vertexArray->setIndexBuffer(
        std::make_unique<IndexBuffer>(_context, id, indices.size(), type, mode));
}

void Mesh::draw(const ShaderProgram& shaderProgram) const
{
    shaderProgram.bind();
    _vertexArray->bind();
    _vertexArray->draw();
    shaderProgram.unbind();
}

} // namespace Runic
