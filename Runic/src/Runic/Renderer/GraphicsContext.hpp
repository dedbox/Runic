#pragma once

#include "Runic/Renderer/Color.hpp"
#include "SDL3/SDL_video.h"
#include "glad/gl.h"
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/ext/vector_int2.hpp"
#include "glm/ext/vector_int3.hpp"
#include "glm/ext/vector_int4.hpp"

namespace Runic
{

using RendererId = uint32_t;

// Buffer Usage ----------------------------------------------------------------

enum class BufferUsage : uint16_t
{
    Transient = GL_STREAM_DRAW,
    Static    = GL_STATIC_DRAW,
    Dynamic   = GL_DYNAMIC_DRAW,
};

static GLenum to_GLenum(BufferUsage usage)
{
    return static_cast<GLenum>(usage);
}

// Vertex Attribute Type -------------------------------------------------------

enum class AttributeType : uint8_t
{
    // clang-format off
    None = 0,
    Bool,
    Int, Int2, Int3, Int4,
    Float, Float2, Float3, Float4,
    Mat3, Mat4,
    // clang-format on
};

static GLenum to_GLenum(AttributeType type)
{
    switch (type)
    {
    case AttributeType::None:
        return GL_NONE;
    case AttributeType::Bool:
        return GL_BOOL;
    case AttributeType::Int:
    case AttributeType::Int2:
    case AttributeType::Int3:
    case AttributeType::Int4:
        return GL_INT;
    case AttributeType::Float:
    case AttributeType::Float2:
    case AttributeType::Float3:
    case AttributeType::Float4:
    case AttributeType::Mat3:
    case AttributeType::Mat4:
        return GL_FLOAT;
    }
    Core::Assert(false, "unknown vertex attribute type");
    return 0;
}

// Index Type ------------------------------------------------------------------

enum class IndexType : uint16_t
{
    Byte  = GL_UNSIGNED_BYTE,
    Short = GL_UNSIGNED_SHORT,
    Int   = GL_UNSIGNED_INT,
};

static GLenum to_GLenum(IndexType type)
{
    return static_cast<GLenum>(type);
}

// Index Mode ------------------------------------------------------------------

enum class IndexMode : uint8_t
{
    Points                 = GL_POINTS,
    Lines                  = GL_LINES,
    LineLoop               = GL_LINE_LOOP,
    LineStrip              = GL_LINE_STRIP,
    Triangles              = GL_TRIANGLES,
    TriangleStrip          = GL_TRIANGLE_STRIP,
    TriangleFan            = GL_TRIANGLE_FAN,
    LinesAdjacency         = GL_LINES_ADJACENCY,
    LineStripAdjacency     = GL_LINE_STRIP_ADJACENCY,
    TrianglesAdjacency     = GL_TRIANGLES_ADJACENCY,
    TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
    Patches                = GL_PATCHES,
};

inline GLenum to_GLenum(IndexMode mode)
{
    return static_cast<GLenum>(mode);
}

// Shader Type -----------------------------------------------------------------

enum class ShaderType : uint16_t
{
    Vertex   = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
};

inline GLenum to_GLenum(ShaderType type)
{
    return static_cast<GLenum>(type);
}

inline std::string format_as(const ShaderType& type)
{
    switch (type)
    {
    case ShaderType::Vertex:
        return "Vertex";
    case ShaderType::Fragment:
        return "Fragment";
    }
    Core::Assert(false, "unknown shader type");
    return "unknown";
}

// Texure Wrap -----------------------------------------------------------------

enum class TextureWrap : uint8_t
{
    Repeat,
    MirrorRepeat,
    ClampEdge,
    ClampBorder,
};

static GLenum to_GLenum(TextureWrap wrap)
{
    switch (wrap)
    {
    case TextureWrap::Repeat:
        return GL_REPEAT;
    case TextureWrap::MirrorRepeat:
        return GL_MIRRORED_REPEAT;
    case TextureWrap::ClampEdge:
        return GL_CLAMP_TO_EDGE;
    case TextureWrap::ClampBorder:
        return GL_CLAMP_TO_BORDER;
    }
    Core::Assert(false, "unknown texture wrap");
    return 0;
}

// Texture Min Filter ----------------------------------------------------------

enum class TextureMinFilter : uint8_t
{
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear,
};

static GLenum to_GLenum(TextureMinFilter filter)
{
    switch (filter)
    {
    case TextureMinFilter::Nearest:
        return GL_NEAREST;
    case TextureMinFilter::Linear:
        return GL_LINEAR;
    case TextureMinFilter::NearestMipmapNearest:
        return GL_NEAREST_MIPMAP_NEAREST;
    case TextureMinFilter::LinearMipmapNearest:
        return GL_LINEAR_MIPMAP_NEAREST;
    case TextureMinFilter::NearestMipmapLinear:
        return GL_NEAREST_MIPMAP_LINEAR;
    case TextureMinFilter::LinearMipmapLinear:
        return GL_LINEAR_MIPMAP_LINEAR;
    }
    Core::Assert(false, "unknown texture min filter");
    return 0;
}

// Texture Mag Filter ----------------------------------------------------------

enum class TextureMagFilter : uint8_t
{
    Nearest,
    Linear,
};

static GLenum to_GLenum(TextureMagFilter filter)
{
    switch (filter)
    {
    case TextureMagFilter::Nearest:
        return GL_NEAREST;
    case TextureMagFilter::Linear:
        return GL_LINEAR;
    }
    Core::Assert(false, "unknown texture mag filter");
    return 0;
}

// Polygon Mode ----------------------------------------------------------------

enum class PolygonMode : uint16_t
{
    Point = GL_POINT,
    Line  = GL_LINE,
    Fill  = GL_FILL,
};

inline GLenum to_GLenum(PolygonMode mode)
{
    return static_cast<GLenum>(mode);
}

// Graphics Context ------------------------------------------------------------

class GraphicsContext
{
public:
    explicit GraphicsContext(SDL_Window* window);

    SDL_GLContext getNative() { return _native; }

    void init();

private:
    SDL_Window* _window;
    SDL_GLContext _native = nullptr;

public:
    void setViewport(const glm::ivec2& size, const glm::ivec2& offset = {0, 0}) const;
    void setClearColor(const glm::vec4& color) const;
    void clear() const;
    // Vertex Array

    RendererId createVertexArray() const;
    void destroyVertexArray(RendererId id) const;

    void bindVertexArray(RendererId id) const;
    void unbindVertexArray() const;

    // Vertex Attribute

    void enableVertexAttribute(uint32_t index) const;
    void defineVertexAttributeData(
        uint32_t index,
        size_t count,
        AttributeType type,
        bool normalize,
        size_t stride,
        const void* offset) const;

    // Buffer

    void destroyBuffer(RendererId id) const;

    // Vertex Buffer

    RendererId createVertexBuffer(const void* data, size_t size, BufferUsage usage) const;

    void bindVertexBuffer(RendererId id) const;
    void unbindVertexBuffer() const;

    // Index Buffer

    RendererId createIndexBuffer(
        const void* data, size_t count, IndexType type, BufferUsage usage) const;

    void bindIndexBuffer(RendererId id) const;
    void unbindIndexBuffer() const;

    // Shader

    RendererId createShader(ShaderType type) const;
    void destroyShader(RendererId id) const;

    bool compileShader(RendererId id, const std::string& source) const;
    std::string getShaderInfoLog(RendererId id) const;

    // Shader Program

    RendererId createShaderProgram() const;
    void destroyShaderProgram(RendererId id) const;

    void attachShader(RendererId programId, RendererId id) const;
    void detachShader(RendererId programId, RendererId id) const;

    bool linkShaderProgram(RendererId id) const;
    std::string getShaderProgramInfoLog(RendererId id) const;

    void useShaderProgram(RendererId id) const;

    // Shader Uniform

    void setUniform(RendererId id, bool value) const;
    void setUniform(RendererId id, int value) const;
    void setUniform(RendererId id, glm::ivec2 value) const;
    void setUniform(RendererId id, glm::ivec3 value) const;
    void setUniform(RendererId id, glm::ivec4 value) const;
    void setUniform(RendererId id, float value) const;
    void setUniform(RendererId id, glm::vec2 value) const;
    void setUniform(RendererId id, glm::vec3 value) const;
    void setUniform(RendererId id, glm::vec4 value) const;
    void setUniform(RendererId id, glm::mat3 value) const;
    void setUniform(RendererId id, glm::mat4 value) const;

    RendererId getUniformLocation(RendererId id, const std::string& name) const;

    // Texture

    RendererId createTexture(SDL_Surface* surface) const;
    void destroyTexture(RendererId id) const;

    void generateMipmap() const;

    void setTextureWrapS(TextureWrap wrap) const;
    void setTextureWrapT(TextureWrap wrap) const;

    void setTextureBorderColor(color color) const;

    void setTextureMinFilter(TextureMinFilter filter) const;
    void setTextureMagFilter(TextureMagFilter filter) const;

    void activateTextureUnit(uint8_t index) const;

    void bindTexture(RendererId id) const;
    void unbindTexture() const;

    // Drawing

    void setPolygonMode(PolygonMode mode) const;

    void drawIndexed(IndexMode mode, size_t count, IndexType type, const void* offset) const;
};

} // namespace Runic
