#pragma once

#include "Runic/Renderer/VertexArray.hpp"

#include "glm/glm.hpp"

namespace Runic
{
class RendererAPI
{
public:
    enum class API
    {
        None   = 0,
        OpenGL = 1,
    };

    virtual ~RendererAPI() = default;

    virtual void setClearColor(const glm::vec4& color) = 0;

    virtual void clear() = 0;

    virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

    static API getAPI() { return s_API; }

private:
    static API s_API;
};
}
