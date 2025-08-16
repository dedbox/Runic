#pragma once

#include "Runic/Renderer/RendererAPI.hpp"

namespace Runic
{
class OpenGLRendererAPI final : public RendererAPI
{
public:
    void setClearColor(const glm::vec4& color) override;

    void clear() override;

    void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
};
} // Runic
