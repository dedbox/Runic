#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"
#include "Runic/Renderer/Texture.hpp"
#include "Runic/Renderer/VertexArray.hpp"

namespace Runic
{

class Mesh
{
public:
    static std::unique_ptr<Mesh> Create(GraphicsContext* context);

    void addVertices(
        const std::vector<float>& vertices,
        const std::vector<VertexAttribute>& layout,
        BufferUsage usage);

    void setIndices(
        const std::vector<uint32_t>& indices, IndexMode mode, IndexType type, BufferUsage usage);

    void addTexture(std::shared_ptr<Texture> texture);

    void draw(const ShaderProgram& shaderProgram) const;

private:
    GraphicsContext* _context;
    std::unique_ptr<VertexArray> _vertexArray;
    std::vector<std::shared_ptr<Texture>> _textures;

    // hide constructor
    explicit Mesh(GraphicsContext* context);
};

} // namespace Runic
