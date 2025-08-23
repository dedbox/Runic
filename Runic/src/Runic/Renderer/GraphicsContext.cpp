#include "GraphicsContext.hpp"

#include "glad/gl.h"

#include "Runic/Core/Log.hpp"
#include "Runic/Core/SDLException.hpp"

namespace Runic
{

GraphicsContext::GraphicsContext(Window* window)
    : _window(window)
    , _context(SDL_GL_CreateContext(_window->getNative()))
{
    if (!_context) throw SDLException("Could not create OpenGL context");

    const int version = gladLoadGL(SDL_GL_GetProcAddress);
    if (!version) throw std::runtime_error("Could not initialize OpenGL loader!");

    // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
    Core::Info("OpenGL initialized!");
    Core::Info("    Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    Core::Info("    Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    Core::Info("    Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
}

void GraphicsContext::setViewport(const glm::ivec2& size, const glm::ivec2& offset)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    glViewport(offset.x, offset.y, size.x, size.y);
}

void GraphicsContext::setClearColor(const glm::vec4& color)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    glClearColor(color.r, color.g, color.b, color.a);
}

void GraphicsContext::clear() const
{
    // NOLINTNEXTLINE(hicpp-signed-bitwise)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsContext::swapBuffers() const
{
    SDL_GL_SwapWindow(_window->getNative());
}

} // namespace Runic
