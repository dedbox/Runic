#pragma once

#include "GraphicsContext.hpp"

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

    Buffer(GraphicsContext* context, RendererId id);
};

// Vertex Buffer ---------------------------------------------------------------

class VertexBuffer : public Buffer
{
public:
    VertexBuffer(GraphicsContext* context, RendererId id, size_t size);

    void bind() const override;
    void unbind() const override;

private:
    size_t _size;
};

// Index Buffer ----------------------------------------------------------------

class IndexBuffer : public Buffer
{
public:
    IndexBuffer(
        GraphicsContext* context, RendererId id, size_t count, IndexType type, IndexMode mode);

    size_t getCount() const { return _count; }
    IndexType getType() const { return _type; }
    IndexMode getMode() const { return _mode; }

    void bind() const override;
    void unbind() const override;

private:
    size_t _count;
    IndexType _type;
    IndexMode _mode;
};

} // namespace Runic
