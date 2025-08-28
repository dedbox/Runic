#include "GraphicsContext.hpp"

#include "glad/gl.h"
#include "glm/gtc/type_ptr.hpp"

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
    case GL_DEBUG_SEVERITY_HIGH: Core::Error(fmt, id, sourceStr, typeStr, msg); break;
    case GL_DEBUG_SEVERITY_MEDIUM: Core::Warn(fmt, id, sourceStr, typeStr, msg); break;
    case GL_DEBUG_SEVERITY_LOW: Core::Info(fmt, id, sourceStr, typeStr, msg); break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
    default: Core::Trace(fmt, id, sourceStr, typeStr, msg); break;
    }
}

} // namespace

// Graphics Context ------------------------------------------------------------

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

// Vertex Array ----------------------------------------------------------------

RendererId GraphicsContext::createVertexArray() const
{
    RendererId id{};
    glGenVertexArrays(1, &id);
    return id;
}

void GraphicsContext::deleteVertexArray(RendererId id) const
{
    Core::Assert(id != 0, "attempt to delete zero array id");
    glDeleteVertexArrays(1, &id);
}

void GraphicsContext::bindVertexArray(RendererId id) const
{
    Core::Assert(id != 0, "attempt to bind a zero array id");
    glBindVertexArray(id);
}

void GraphicsContext::unbindVertexArray() const
{
    glBindVertexArray(0);
}

// Vertex Attribute ------------------------------------------------------------

void GraphicsContext::enableVertexAttribute(uint32_t index) const
{
    glEnableVertexAttribArray(index);
}

void GraphicsContext::defineVertexAttributeData(
    uint32_t index,
    size_t count,
    ElementType type,
    bool normalize,
    size_t stride,
    const void* offset) const
{
    glVertexAttribPointer(
        index, static_cast<GLint>(count), ElementTypeToGLenum(type), normalize ? GL_TRUE : GL_FALSE,
        static_cast<GLsizei>(stride), offset);
}

// Vertex Buffer ---------------------------------------------------------------

RendererId
GraphicsContext::createVertexBuffer(const void* data, size_t size, BufferUsage usage) const
{
    RendererId id{};
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, BufferUsageToGLenum(usage));
    return id;
}

void GraphicsContext::bindVertexBuffer(RendererId id) const
{
    Core::Assert(id != 0, "attempt to bind non-zero vertex buffer id");
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void GraphicsContext::unbindVertexBuffer() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Index Buffer ----------------------------------------------------------------

RendererId GraphicsContext::createIndexBuffer(
    const void* data, size_t count, IndexType type, BufferUsage usage) const
{
    RendererId id{};
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(IndexTypeToGLenum(type))),
        data, BufferUsageToGLenum(usage));
    return id;
}

void GraphicsContext::bindIndexBuffer(RendererId id) const
{
    Core::Assert(id != 0, "attempt to bind non-zero index buffer id");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void GraphicsContext::unbindIndexBuffer() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Buffer (both) ---------------------------------------------------------------

void GraphicsContext::deleteBuffer(RendererId id) const
{
    Core::Assert(id != 0, "attempt to delete non-zero buffer id");
    glDeleteBuffers(1, &id);
}

// Shader ----------------------------------------------------------------------

RendererId GraphicsContext::createShader(ShaderType type) const
{
    return glCreateShader(ShaderTypeToGLenum(type));
}

void GraphicsContext::deleteShader(RendererId id) const
{
    glDeleteShader(id);
}

bool GraphicsContext::compileShader(RendererId id, const std::string& source) const
{
    const char* raw_source = source.c_str();
    glShaderSource(id, 1, &raw_source, nullptr);
    glCompileShader(id);

    GLint succeeded = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &succeeded);
    return succeeded == GL_TRUE;
}

std::string GraphicsContext::getShaderInfoLog(RendererId id) const
{
    GLint length = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

    std::vector<GLchar> log(length);
    glGetShaderInfoLog(id, length, &length, &log[0]);

    return log.data();
}

// Shader Program --------------------------------------------------------------

RendererId GraphicsContext::createShaderProgram() const
{
    return glCreateProgram();
}

void GraphicsContext::deleteShaderProgram(RendererId id) const
{
    glDeleteProgram(id);
}

void GraphicsContext::attachShader(RendererId programId, RendererId id) const
{
    glAttachShader(programId, id);
}

void GraphicsContext::detachShader(RendererId programId, RendererId id) const
{
    glDetachShader(programId, id);
}

bool GraphicsContext::linkShaderProgram(RendererId id) const
{
    glLinkProgram(id);

    GLint succeeded = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &succeeded);
    return succeeded == GL_TRUE;
}

std::string GraphicsContext::getShaderProgramInfoLog(RendererId id) const
{
    GLint length = 0;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

    std::vector<GLchar> log(length);
    glGetProgramInfoLog(id, length, &length, &log[0]);

    return log.data();
}

void GraphicsContext::useShaderProgram(RendererId id) const
{
    glUseProgram(id);
}

// Shader Uniform --------------------------------------------------------------

// NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)

void GraphicsContext::setUniformBool(RendererId id, bool value) const
{
    glUniform1ui(static_cast<GLint>(id), value);
}

void GraphicsContext::setUniformInt(RendererId id, int value) const
{
    glUniform1i(static_cast<GLint>(id), value);
}

void GraphicsContext::setUniformInt2(RendererId id, glm::ivec2 value) const
{
    glUniform2i(static_cast<GLint>(id), value.x, value.y);
}

void GraphicsContext::setUniformInt3(RendererId id, glm::ivec3 value) const
{
    glUniform3i(static_cast<GLint>(id), value.x, value.y, value.z);
}

void GraphicsContext::setUniformInt4(RendererId id, glm::ivec4 value) const
{
    glUniform4i(static_cast<GLint>(id), value.x, value.y, value.z, value.w);
}

void GraphicsContext::setUniformFloat(RendererId id, float value) const
{
    glUniform1f(static_cast<GLint>(id), value);
}

void GraphicsContext::setUniformFloat2(RendererId id, glm::vec2 value) const
{
    glUniform2f(static_cast<GLint>(id), value.x, value.y);
}

void GraphicsContext::setUniformFloat3(RendererId id, glm::vec3 value) const
{
    glUniform3f(static_cast<GLint>(id), value.x, value.y, value.z);
}

void GraphicsContext::setUniformFloat4(RendererId id, glm::vec4 value) const
{
    glUniform4f(static_cast<GLint>(id), value.x, value.y, value.z, value.w);
}

void GraphicsContext::setUniformMat3(RendererId id, glm::mat3 value) const
{
    glUniformMatrix3fv(static_cast<GLint>(id), 1, GL_FALSE, glm::value_ptr(value));
}

void GraphicsContext::setUniformMat4(RendererId id, glm::mat4 value) const
{
    glUniformMatrix4fv(static_cast<GLint>(id), 1, GL_FALSE, glm::value_ptr(value));
}

// NOLINTEND(cppcoreguidelines-pro-type-union-access)

RendererId GraphicsContext::getUniformLocation(RendererId id, const std::string& name) const
{
    int location = glGetUniformLocation(id, name.c_str());
    if (location == -1) Core::Error("Could not find uniform `{}'!", name);
    return location;
}

// Drawing ---------------------------------------------------------------------

void GraphicsContext::setPolygonMode(PolygonMode mode) const
{
    glPolygonMode(GL_FRONT_AND_BACK, PolygonModeToGLenum(mode));
}

void GraphicsContext::drawElements(
    IndexMode mode, size_t count, IndexType type, const void* offset) const
{
    glDrawElements(
        IndexModeToGLenum(mode), static_cast<GLsizei>(count), IndexTypeToGLenum(type), offset);
}

} // namespace Runic
