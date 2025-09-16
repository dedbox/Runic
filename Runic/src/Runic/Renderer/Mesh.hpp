#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"
#include "Runic/Renderer/Texture.hpp"
#include "Runic/Renderer/VertexArray.hpp"

namespace Runic
{

class Mesh
{
protected:
    GraphicsContext* _context;

    Mesh(GraphicsContext* context, DrawMode mode)
        : _context(context)
        , _vertexArray(VertexArray::Create(_context, mode))
    {
    }

public:
    static std::unique_ptr<Mesh> Create(GraphicsContext* context, DrawMode mode)
    {
        return std::unique_ptr<Mesh>(new Mesh(context, mode));
    }

    void addVertexBuffer(
        std::unique_ptr<VertexBuffer> vertexBuffer, const std::vector<VertexAttribute>& layout)
    {
        _vertexArray->addVertexBuffer(std::move(vertexBuffer), layout);
    }

    void setIndices(const std::vector<uint32_t>& indices, IndexType type, BufferUsage usage)
    {
        RendererId id = _context->createIndexBuffer(indices.data(), indices.size(), type, usage);
        _vertexArray->setIndexBuffer(IndexBuffer::Create(_context, indices, type, usage));
    }

    void addTexture(const std::string& name, std::shared_ptr<Texture> texture)
    {
        if (texture)
            _textures.emplace_back(name, std::move(texture));
    }

    void draw(const ShaderProgram& shaderProgram) const
    {
        shaderProgram.bind();

        for (const auto&& [i, binding] : _textures | std::ranges::views::enumerate)
        {
            const auto& [name, texture] = binding;
            _context->activateTextureUnit(i);
            shaderProgram.setUniform(name, static_cast<int>(i));
            texture->bind();
        }

        _vertexArray->bind();
        _vertexArray->draw();
        _vertexArray->unbind();

        shaderProgram.unbind();
    }

private:
    std::unique_ptr<VertexArray> _vertexArray;
    std::vector<std::pair<std::string, std::shared_ptr<Texture>>> _textures;
};

} // namespace Runic
