#include "Mesh.hpp"

namespace Runic
{

Mesh::Mesh(Renderer* renderer)
    : _renderer(renderer)
{
    _vertexArray = _renderer->createVertexArray();
}

void Mesh::addVertices(
    const std::vector<float>& vertices,
    const std::vector<VertexAttribute>& layout,
    BufferUsage usage)
{
    auto vertexBuffer =
        _renderer->createVertexBuffer(vertices.data(), vertices.size() * sizeof(float), usage);
    _vertexArray->addVertexBuffer(std::move(vertexBuffer), layout);
}

void Mesh::setIndices(const std::vector<uint32_t>& indices, IndexMode mode, BufferUsage usage)
{
    auto indexBuffer =
        _renderer->createIndexBuffer(indices.data(), indices.size(), IndexType::Int, mode, usage);
    _vertexArray->setIndexBuffer(std::move(indexBuffer));
}

void Mesh::draw(const ShaderProgram& shaderProgram) const
{
    shaderProgram.use();
    _vertexArray->bind();
    _vertexArray->draw();
}

} // namespace Runic
