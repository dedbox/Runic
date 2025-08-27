#pragma once

#include "Runic/Renderer/Buffer.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{

// Buffer Layout ---------------------------------------------------------------

struct LayoutElement
{
    ElementType type;
    bool normalized;
};

// Vertex Array ----------------------------------------------------------------

class VertexArray
{
public:
    VertexArray(GraphicsContext* gc, RendererId id);

    // prevent copying
    VertexArray(VertexArray&)                  = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    // allow moving
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addVertexBuffer(
        std::unique_ptr<VertexBuffer> vertexBuffer, const std::vector<LayoutElement>& layout);

    void setIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer);

private:
    GraphicsContext* _gc;
    RendererId _id;
    std::vector<std::unique_ptr<VertexBuffer>> _vertexBuffers;
    std::unique_ptr<IndexBuffer> _indexBuffer;
};

} // namespace Runic
