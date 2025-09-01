#include "Runic/Renderer/Mesh.hpp"

namespace Runic
{

std::unique_ptr<Mesh> Mesh::Create(GraphicsContext* context)
{
    return std::unique_ptr<Mesh>(new Mesh(context));
}

Mesh::Mesh(GraphicsContext* context)
    : _context(context)
{
    _vertexArray = VertexArray::Create(_context);
}

void Mesh::addVertices(
    const std::vector<float>& vertices,
    const std::vector<VertexAttribute>& layout,
    BufferUsage usage)
{
    size_t size = vertices.size() * sizeof(float);
    _vertexArray->addVertexBuffer(
        VertexBuffer::Create(_context, vertices.data(), size, usage), layout);
}

void Mesh::setIndices(
    const std::vector<uint32_t>& indices, IndexMode mode, IndexType type, BufferUsage usage)
{
    RendererId id = _context->createIndexBuffer(indices.data(), indices.size(), type, usage);
    _vertexArray->setIndexBuffer(
        IndexBuffer::Create(_context, indices.data(), indices.size(), mode, type, usage));
}

void Mesh::draw(const ShaderProgram& shaderProgram) const
{
    shaderProgram.bind();
    _vertexArray->bind();
    _vertexArray->draw();
    shaderProgram.unbind();
}

} // namespace Runic
