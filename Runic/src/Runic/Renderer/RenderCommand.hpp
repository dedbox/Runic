#pragma once

#include "Runic/Renderer/RendererAPI.hpp"

namespace Runic
{
class RenderCommand
{
public:
    static void setClearColor(const glm::vec4& color) { s_rendererAPI->setClearColor(color); };

    static void clear() { s_rendererAPI->clear(); }

    static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
    {
        s_rendererAPI->drawIndexed(vertexArray);
    }

private:
    static RendererAPI* s_rendererAPI;
};
}
