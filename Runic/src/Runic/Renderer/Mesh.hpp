#pragma once

#include "GraphicsContext.hpp"
#include "Renderer.hpp"
#include "VertexArray.hpp"

namespace Runic
{

class Mesh
{
public:
    explicit Mesh(Renderer* renderer);

    void addVertices(
        const std::vector<float>& vertices,
        const std::vector<LayoutElement>& layout,
        BufferUsage usage);

    void setIndices(const std::vector<uint32_t>& indices, IndexMode mode, BufferUsage usage);

    void draw(const ShaderProgram& shaderProgram) const;

private:
    Renderer* _renderer;
    std::unique_ptr<VertexArray> _vertexArray;
};

} // namespace Runic
