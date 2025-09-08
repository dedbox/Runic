#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"
#include "Runic/Renderer/Texture.hpp"
#include "Runic/Renderer/VertexArray.hpp"

namespace Runic
{

template <typename... Layouts>
class Mesh
{
protected:
    GraphicsContext* _context;

    Mesh(GraphicsContext* context, DrawMode mode)
        : _context(context)
        , _vertexArray(VertexArray<Layouts...>::Create(_context, mode))
    {
    }

public:
    static std::unique_ptr<Mesh> Create(GraphicsContext* context, DrawMode mode)
    {
        return std::unique_ptr<Mesh>(new Mesh(context, mode));
    }

    void setVertexBuffers(VertexBufferTuple<Layouts...> vertexBuffers)
    {
        _vertexArray->setVertexBuffers(std::move(vertexBuffers));
    }

    void setIndices(const std::vector<uint32_t>& indices, IndexType type, BufferUsage usage)
    {
        RendererId id = _context->createIndexBuffer(indices.data(), indices.size(), type, usage);
        _vertexArray->setIndexBuffer(IndexBuffer::Create(_context, indices, type, usage));
    }

    void addTexture(std::shared_ptr<Texture> texture)
    {
        if (texture)
            _textures.push_back(std::move(texture));
    }

    void draw(const ShaderProgram<Layouts...>& shaderProgram) const
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

private:
    std::unique_ptr<VertexArray<Layouts...>> _vertexArray;
    std::vector<std::shared_ptr<Texture>> _textures;
};

} // namespace Runic
