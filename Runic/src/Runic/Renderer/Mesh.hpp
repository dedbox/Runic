#pragma once

#include "GraphicsContext.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"
#include "VertexArray.hpp"

namespace Runic
{

class Mesh
{
public:
    explicit Mesh(GraphicsContext* context);

    void addVertices(
        const std::vector<float>& vertices,
        const std::vector<VertexAttribute>& layout,
        BufferUsage usage);

    void setIndices(
        const std::vector<uint32_t>& indices, IndexMode mode, IndexType type, BufferUsage usage);

    void draw(const ShaderProgram& shaderProgram) const;

private:
    GraphicsContext* _context;
    std::unique_ptr<VertexArray> _vertexArray;
};

} // namespace Runic
