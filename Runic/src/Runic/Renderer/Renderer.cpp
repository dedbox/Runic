#include "Renderer.hpp"

#include "glad/gl.h"

namespace Runic
{

Renderer::Renderer(const GraphicsContext& gc)
    : _gc(gc)
{
}

void Renderer::beginFrame() {}

void Renderer::endFrame()
{
    SDL_GL_SwapWindow(_gc->getWindow().getNative());
}

void Renderer::setViewport(const glm::ivec2& size, const glm::ivec2& offset) const
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    glViewport(offset.x, offset.y, size.x, size.y);
}

void Renderer::setClearColor(const glm::vec4& color) const
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::clear() const
{
    // NOLINTNEXTLINE(hicpp-signed-bitwise)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // namespace Runic
