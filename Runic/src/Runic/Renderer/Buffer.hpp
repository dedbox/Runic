#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{

class Buffer
{
public:
    virtual ~Buffer();

    // allow moving
    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;

    // prevent copying
    Buffer(const Buffer&)            = delete;
    Buffer& operator=(const Buffer&) = delete;

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

protected:
    GraphicsContext* _context;
    RendererId _id;

    // hide constructor
    Buffer(GraphicsContext* context, RendererId id);
};

// Vertex Buffer ---------------------------------------------------------------

class VertexBuffer : public Buffer
{
public:
    static std::unique_ptr<VertexBuffer> Create(
        GraphicsContext* context, const void* data, size_t size, BufferUsage usage);

    void bind() const override;
    void unbind() const override;

private:
    size_t _size;

    // hide constructor
    VertexBuffer(GraphicsContext* context, RendererId id, size_t size);
};

// Index Buffer ----------------------------------------------------------------

class IndexBuffer : public Buffer
{
public:
    static std::unique_ptr<IndexBuffer> Create(
        GraphicsContext* context,
        const void* data,
        size_t count,
        IndexMode mode,
        IndexType type,
        BufferUsage usage);

    size_t getCount() const { return _count; }
    IndexType getType() const { return _type; }
    IndexMode getMode() const { return _mode; }

    void bind() const override;
    void unbind() const override;

private:
    size_t _count;
    IndexType _type;
    IndexMode _mode;

    IndexBuffer(
        GraphicsContext* context, RendererId id, size_t count, IndexType type, IndexMode mode);
};

} // namespace Runic
