#include "pch.hpp"
#include "Runic/Renderer/VertexArray.hpp"

#include "Platform/OpenGL/OpenGLVertexArray.hpp"
#include "Runic/Renderer/Renderer.hpp"

namespace Runic
{
VertexArray* VertexArray::create()
{
    switch (Renderer::getAPI()) {
        case RendererAPI::None:
            RUNIC_CORE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;

        case RendererAPI::OpenGL:
            return new OpenGLVertexArray();
    }
    RUNIC_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
}
