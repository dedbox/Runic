#pragma once

#include "Runic/Renderer/Buffer.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{

// Buffer Layout ---------------------------------------------------------------

struct VertexAttribute
{
    AttributeType type;
    bool normalize;
};

// Vertex Array ----------------------------------------------------------------

class VertexArray
{
public:
    VertexArray(GraphicsContext* gc, RendererId id);

    // allow moving
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    // prevent copying
    VertexArray(VertexArray&)                  = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addVertexBuffer(
        std::unique_ptr<VertexBuffer> vertexBuffer, const std::vector<VertexAttribute>& layout);

    void setIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer);

    void draw() const;

private:
    GraphicsContext* _gc;
    RendererId _id;
    std::vector<std::unique_ptr<VertexBuffer>> _vertexBuffers;
    std::unique_ptr<IndexBuffer> _indexBuffer;
};

} // namespace Runic
