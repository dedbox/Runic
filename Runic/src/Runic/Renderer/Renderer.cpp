#include "Renderer.hpp"

namespace Runic
{

Renderer::Renderer(std::unique_ptr<GraphicsContext> gc)
    : _gc(std::move(gc))
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

std::unique_ptr<VertexArray> Renderer::createVertexArray() const
{
    RendererId id = _gc->createVertexArray();
    return std::make_unique<VertexArray>(_gc.get(), id);
}

std::unique_ptr<VertexBuffer>
Renderer::createVertexBuffer(const void* data, size_t size, BufferUsage usage)
{
    RendererId id = _gc->createVertexBuffer(data, size, usage);
    return std::make_unique<VertexBuffer>(_gc.get(), id, size);
}

std::unique_ptr<IndexBuffer>
Renderer::createIndexBuffer(const void* data, size_t count, IndexType type, BufferUsage usage)
{
    RendererId id = _gc->createIndexBuffer(data, count, type, usage);
    return std::make_unique<IndexBuffer>(_gc.get(), id, count, type);
}

} // namespace Runic
