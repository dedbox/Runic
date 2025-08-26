#pragma once

#include "glm/ext/vector_float4.hpp"
#include "glm/ext/vector_int2.hpp"

#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{

class Renderer
{
public:
    explicit Renderer(const GraphicsContext& context);

    GraphicsContext& getGraphicsContext() { return *_gc; }

    void beginFrame();
    void endFrame();

    void setViewport(const glm::ivec2& size, const glm::ivec2& offset = {0, 0}) const;
    void setClearColor(const glm::vec4& color) const;
    void clear() const;

private:
    std::optional<GraphicsContext> _gc;
};

} // namespace Runic
