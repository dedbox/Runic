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
    template <typename T>
    static std::unique_ptr<VertexBuffer> Create(
        GraphicsContext* context, const std::vector<T>& elements, BufferUsage usage)
    {
        size_t count  = elements.size();
        size_t size   = count * sizeof(T);
        RendererId id = context->createVertexBuffer(elements.data(), size, usage);
        return std::unique_ptr<VertexBuffer>(new VertexBuffer(context, id, count, size));
    }

    size_t getCount() const { return _count; }
    size_t getSize() const { return _size; }

    void bind() const override;
    void unbind() const override;

private:
    size_t _count;
    size_t _size;

    // hide constructor
    VertexBuffer(GraphicsContext* context, RendererId id, size_t count, size_t size);
};

// Index Buffer ----------------------------------------------------------------

class IndexBuffer : public Buffer
{
public:
    template <typename T>
    static std::unique_ptr<IndexBuffer> Create(
        GraphicsContext* context, const std::vector<T>& elements, IndexType type, BufferUsage usage)
    {
        size_t count  = elements.size();
        size_t size   = count * sizeof(T);
        RendererId id = context->createIndexBuffer(elements.data(), count, type, usage);
        return std::unique_ptr<IndexBuffer>(new IndexBuffer(context, id, count, size, type));
    }

    size_t getCount() const { return _count; }
    size_t getSize() const { return _size; }
    IndexType getType() const { return _type; }

    void bind() const override;
    void unbind() const override;

private:
    size_t _count;
    size_t _size;
    IndexType _type;

    IndexBuffer(GraphicsContext* context, RendererId id, size_t count, size_t size, IndexType type);
};

} // namespace Runic
