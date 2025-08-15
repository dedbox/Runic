#include "pch.hpp"
#include "Runic/Renderer/Buffer.hpp"

#include "Platform/OpenGL/OpenGLBuffer.hpp"
#include "Runic/Renderer/Renderer.hpp"

namespace Runic
{
VertexBuffer* VertexBuffer::create(const float* vertices, const uint32_t size)
{
    switch (Renderer::getAPI()) {
        case RendererAPI::None:
            RUNIC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;

        case RendererAPI::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
    }

    RUNIC_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

IndexBuffer* IndexBuffer::create(const uint32_t* indices, const uint32_t count)
{
    switch (Renderer::getAPI()) {
        case RendererAPI::None:
            RUNIC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;

        case RendererAPI::OpenGL:
            return new OpenGLIndexBuffer(indices, count);
    }

    RUNIC_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
}
