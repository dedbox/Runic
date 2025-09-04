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
    static std::unique_ptr<VertexArray> Create(GraphicsContext* context, DrawMode _mode);

    // allow moving
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    // prevent copying
    VertexArray(VertexArray&)                  = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    ~VertexArray();

    DrawMode getMode() const { return _mode; }

    void bind() const;
    void unbind() const;

    void addVertexBuffer(
        std::unique_ptr<VertexBuffer> vertexBuffer, const std::vector<VertexAttribute>& layout);

    void setIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer);

    void draw() const;

private:
    GraphicsContext* _context;
    RendererId _id;
    DrawMode _mode;
    size_t _count = 0;
    std::vector<std::unique_ptr<VertexBuffer>> _vertexBuffers;
    std::unique_ptr<IndexBuffer> _indexBuffer;

    // hide constructor
    VertexArray(GraphicsContext* context, RendererId id, DrawMode _mode);
};

} // namespace Runic
