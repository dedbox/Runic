#include "Runic/Renderer/Mesh.hpp"

namespace Runic
{

std::unique_ptr<Mesh> Mesh::Create(GraphicsContext* context, DrawMode mode)
{
    return std::unique_ptr<Mesh>(new Mesh(context, mode));
}

Mesh::Mesh(GraphicsContext* context, DrawMode mode)
    : _context(context)
    , _vertexArray(VertexArray::Create(_context, mode))
{
}

void Mesh::setIndices(const std::vector<uint32_t>& indices, IndexType type, BufferUsage usage)
{
    RendererId id = _context->createIndexBuffer(indices.data(), indices.size(), type, usage);
    _vertexArray->setIndexBuffer(IndexBuffer::Create(_context, indices, type, usage));
}

void Mesh::addTexture(std::shared_ptr<Texture> texture)
{
    if (texture)
        _textures.push_back(std::move(texture));
}

void Mesh::draw(const ShaderProgram& shaderProgram) const
{
    shaderProgram.bind();

    for (const auto&& [index, texture] : _textures | std::ranges::views::enumerate)
    {
        _context->activateTextureUnit(index);
        texture->bind();
    }

    _vertexArray->bind();

    _vertexArray->draw();

    _vertexArray->unbind();
    shaderProgram.unbind();
}

} // namespace Runic
