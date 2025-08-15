#pragma once

#include "Runic/Renderer/Buffer.hpp"

namespace Runic
{
class OpenGLVertexBuffer final : public VertexBuffer
{
public:
    OpenGLVertexBuffer(const float* vertices, uint32_t size);

    ~OpenGLVertexBuffer() override;

    void bind() const override;

    void unbind() const override;

private:
    uint32_t _rendererID;
};

class OpenGLIndexBuffer final : public IndexBuffer
{
public:
    OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);

    ~OpenGLIndexBuffer() override;

    void bind() const override;

    void unbind() const override;

    uint32_t getCount() const override { return _count; }

private:
    uint32_t _rendererID;
    uint32_t _count;
};
}
