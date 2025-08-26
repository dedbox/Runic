#include "GraphicsContext.hpp"

#include "SDL3/SDL_video.h"
#include "glad/gl.h"

// TODO add SDLException to precompiled headers
#include "Runic/Core/SDLException.hpp"

namespace Runic
{

namespace
{

void GLAPIENTRY GLDebugMessageCallback(
    // NOLINTBEGIN
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    // NOLINTEND
    const GLchar* msg,
    const void* /*data*/)
{
    std::string sourceStr, typeStr;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API: sourceStr = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "WINDOW SYSTEM"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "SHADER COMPILER"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "THIRD PARTY"; break;
    case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "APPLICATION"; break;
    case GL_DEBUG_SOURCE_OTHER: sourceStr = "OTHER"; break;
    default: sourceStr = "UNKNOWN"; break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: typeStr = "ERROR"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "DEPRECATED BEHAVIOR"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "UDEFINED BEHAVIOR"; break;
    case GL_DEBUG_TYPE_PORTABILITY: typeStr = "PORTABILITY"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: typeStr = "PERFORMANCE"; break;
    case GL_DEBUG_TYPE_OTHER: typeStr = "OTHER"; break;
    case GL_DEBUG_TYPE_MARKER: typeStr = "MARKER"; break;
    default: typeStr = "UNKNOWN"; break;
    }

    std::string fmt = "GLDEBUG({}) [{} {}]: {}";

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH: Core::Error(fmt, id, sourceStr, typeStr, msg);
    case GL_DEBUG_SEVERITY_MEDIUM: Core::Warn(fmt, id, sourceStr, typeStr, msg);
    case GL_DEBUG_SEVERITY_LOW: Core::Info(fmt, id, sourceStr, typeStr, msg);
    case GL_DEBUG_SEVERITY_NOTIFICATION:
    default: Core::Trace(fmt, id, sourceStr, typeStr, msg);
    }
}

} // namespace

GraphicsContext::GraphicsContext(const Window& window)
    : _window(window)
{
}

void GraphicsContext::init()
{
    _native = SDL_GL_CreateContext(_window.getNative());

    if (!_native) throw SDLException("Could not create OpenGL context");

    const int version = gladLoadGL(SDL_GL_GetProcAddress);
    if (!version) throw std::runtime_error("Could not initialize OpenGL loader!");

    // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
    Core::Info("OpenGL initialized!");
    Core::Info("    Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    Core::Info("    Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    Core::Info("    Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

    if constexpr (Core::Debugging)
    {
        GLint context_flags{0};
        glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
        if (static_cast<uint32_t>(context_flags) & static_cast<uint32_t>(GL_CONTEXT_FLAG_DEBUG_BIT))
        {
            Core::Info("Debug context created.");
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(GLDebugMessageCallback, nullptr);
        }
        else
            Core::Warn("Debug context NOT created.");
    }
}

} // namespace Runic
