#pragma once

#include "Runic/Renderer/VertexArray.hpp"

namespace Runic
{
class OpenGLVertexArray final : public VertexArray
{
public:
    OpenGLVertexArray();

    ~OpenGLVertexArray() override;

    void bind() const override;

    void unbind() const override;

    void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;

    void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    const std::vector<std::shared_ptr<VertexBuffer> >& getVertexBuffers() const override { return _vertexBuffers; }

    const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override { return _indexBuffer; }

private:
    uint32_t _rendererID;
    std::vector<std::shared_ptr<VertexBuffer> > _vertexBuffers;
    std::shared_ptr<IndexBuffer> _indexBuffer;
};
} // Runic
