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

template <typename Layout>
class VertexArray
{
private:
    GraphicsContext* _context;
    RendererId _id;
    DrawMode _mode;

    VertexArray(GraphicsContext* context, RendererId id, DrawMode mode)
        : _context(context)
        , _id(id)
        , _mode(mode)
    {
        bind();
    }

public:
    static std::unique_ptr<VertexArray> Create(GraphicsContext* context, DrawMode mode)
    {
        RendererId id = context->createVertexArray();
        return std::unique_ptr<VertexArray>(new VertexArray(context, id, mode));
    }

    // allow moving
    VertexArray(VertexArray&& other) noexcept
        : _context(std::exchange(other._context, nullptr))
        , _id(std::exchange(other._id, 0))
        , _mode(other._mode)
    {
    }

    VertexArray& operator=(VertexArray&& other) noexcept
    {
        if (this != &other)
        {
            _context->destroyVertexArray(_id);
            _context = std::exchange(other._context, nullptr);
            _id      = std::exchange(other._id, 0);
            _mode    = other._mode;
        }
        return *this;
    }

    // prevent copying
    VertexArray(VertexArray&)                  = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    ~VertexArray()
    {
        if (_id != 0)
        {
            _context->destroyVertexArray(_id);
            _id = 0;
        }
    }

    DrawMode getMode() const { return _mode; }

    void bind() const { _context->bindVertexArray(_id); }
    void unbind() const { _context->unbindVertexArray(); }

    void addVertexBuffer(std::unique_ptr<VertexBuffer> vertexBuffer)
    {
        _count += vertexBuffer->getSize() / Layout::stride;

        for (const auto&& [index, count, type, normalize, offset] : std::ranges::views::zip(
                 std::ranges::views::iota(0),
                 Layout::counts,
                 Layout::attributeTypes,
                 Layout::normalizes,
                 Layout::offsets))
        {
            _context->enableVertexAttribute(index);
            _context->defineVertexAttributeData(
                index,
                count,
                type,
                normalize,
                Layout::stride,
                reinterpret_cast<const void*>(offset)); // NOLINT
        }

        _vertexBuffers.push_back(std::move(vertexBuffer));
    }

    void setIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer)
    {
        _indexBuffer = std::move(indexBuffer);
    }

    void draw() const
    {
        if (_indexBuffer)
            _context->drawIndexed(
                _mode, _indexBuffer->getCount(), _indexBuffer->getType(), nullptr);
        else
        {
            _context->drawVertices(_mode, 0, _count);
        }
    }

private:
    size_t _count = 0;
    std::vector<std::unique_ptr<VertexBuffer>> _vertexBuffers;
    std::unique_ptr<IndexBuffer> _indexBuffer;
};

} // namespace Runic
