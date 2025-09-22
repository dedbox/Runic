#pragma once

#include "Runic/Renderer/CubeMap.hpp"
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

    void addTexture(const std::string& name, const std::shared_ptr<Texture>& texture)
    {
        if (texture)
            _textures.emplace_back(name, texture);
    }

    void addCubeMap(const std::string& name, const std::shared_ptr<CubeMap>& cubeMap)
    {
        if (cubeMap)
            _cubeMaps.emplace_back(name, cubeMap);
    }

    void draw(const ShaderProgram& shaderProgram) const
    {
        shaderProgram.bind();
        _vertexArray->bind();

        for (const auto&& [i, binding] : _textures | std::ranges::views::enumerate)
        {
            const auto& [name, texture] = binding;
            _context->activateTextureUnit(i);
            shaderProgram.setUniform(name, static_cast<int>(i));
            texture->bind();
        }

        const size_t n = _textures.size();

        for (const auto&& [i, binding] : _cubeMaps | std::ranges::views::enumerate)
        {
            _context->activateTextureUnit(n + i);
            const auto& [name, cubeMap] = binding;
            shaderProgram.setUniform(name, static_cast<int>(n + i));
            cubeMap->bind();
        }

        _vertexArray->draw();

        _vertexArray->unbind();
        shaderProgram.unbind();
    }

private:
    std::unique_ptr<VertexArray> _vertexArray;
    std::vector<std::pair<std::string, std::shared_ptr<Texture>>> _textures;
    std::vector<std::pair<std::string, std::shared_ptr<CubeMap>>> _cubeMaps;
};

} // namespace Runic
