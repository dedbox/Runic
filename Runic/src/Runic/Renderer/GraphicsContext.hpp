#pragma once

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
#include "glm/ext/vector_uint2.hpp"

#include "Runic/Renderer/Color.hpp"

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

static std::string to_string(AttributeType type)
{
    // clang-format off
    switch (type)
    {
    case AttributeType::None: return "None";
    case AttributeType::Bool: return "Bool";
    case AttributeType::Int: return "Int";
    case AttributeType::Int2: return "Int2";
    case AttributeType::Int3: return "Int3";
    case AttributeType::Int4: return "Int4";
    case AttributeType::Float: return "Float";
    case AttributeType::Float2: return "Float2";
    case AttributeType::Float3: return "Float3";
    case AttributeType::Float4: return "Float4";
    case AttributeType::Mat3: return "Mat3";
    case AttributeType::Mat4: return "Mat4";
    }
    // clang-format on
    Core::Assert(false, "unknown vertex attribute type");
    return "unknown";
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

// Draw Mode ------------------------------------------------------------------

enum class DrawMode : uint8_t
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

inline GLenum to_GLenum(DrawMode mode)
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

// Depth Test Function ---------------------------------------------------------

enum class DepthFunction : uint16_t
{
    Never    = GL_NEVER,
    Less     = GL_LESS,
    Equal    = GL_EQUAL,
    LEqual   = GL_LEQUAL,
    Greater  = GL_GREATER,
    NotEqual = GL_NOTEQUAL,
    GEqual   = GL_GEQUAL,
    Always   = GL_ALWAYS,
};

inline GLenum to_GLenum(DepthFunction fun)
{
    return static_cast<GLenum>(fun);
}

// Stencil Test Function -------------------------------------------------------

enum class StencilFunction : uint16_t
{
    Never    = GL_NEVER,
    Less     = GL_LESS,
    LEqual   = GL_LEQUAL,
    Greater  = GL_GREATER,
    GEqual   = GL_GEQUAL,
    Equal    = GL_EQUAL,
    NotEqual = GL_NOTEQUAL,
    Always   = GL_ALWAYS,
};

inline GLenum to_GLenum(StencilFunction fun)
{
    return static_cast<GLenum>(fun);
}

// Stencil Operation -----------------------------------------------------------

enum class StencilOp : uint16_t
{
    Keep          = GL_KEEP,
    Zero          = GL_ZERO,
    Replace       = GL_REPLACE,
    Increment     = GL_INCR,
    IncrementWrap = GL_INCR_WRAP,
    Decrement     = GL_DECR,
    DecrementWrap = GL_DECR_WRAP,
    Invert        = GL_INVERT,
};

inline GLenum to_GLenum(StencilOp op)
{
    return static_cast<GLenum>(op);
}

// Blend Factor ----------------------------------------------------------------

enum class BlendFactor
{
    Zero                  = GL_ZERO,
    One                   = GL_ONE,
    SrcColor              = GL_SRC_COLOR,
    OneMinusSrcColor      = GL_ONE_MINUS_SRC_COLOR,
    DstColor              = GL_DST_COLOR,
    OneMinusDstColor      = GL_ONE_MINUS_DST_COLOR,
    SrcAlpha              = GL_SRC_ALPHA,
    OneMinusSrcAlpha      = GL_ONE_MINUS_SRC_ALPHA,
    DstAlpha              = GL_DST_ALPHA,
    OneMinusDstAlpha      = GL_ONE_MINUS_DST_ALPHA,
    ConstantColor         = GL_CONSTANT_COLOR,
    OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
    ConstantAlpha         = GL_CONSTANT_ALPHA,
    OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA,
};

inline GLenum to_GLenum(BlendFactor factor)
{
    return static_cast<GLenum>(factor);
}

// Cull Mode -------------------------------------------------------------------

enum class CullMode : uint16_t
{
    Front     = GL_FRONT,
    Back      = GL_BACK,
    FrontBack = GL_FRONT_AND_BACK,
};

inline GLenum to_GLenum(CullMode mode)
{
    return static_cast<GLenum>(mode);
}

// Front Face ------------------------------------------------------------------

enum class FrontFace : uint16_t
{
    CW  = GL_CW,
    CCW = GL_CCW,
};

inline GLenum to_GLenum(FrontFace face)
{
    return static_cast<GLenum>(face);
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
    // Viewport
    void setViewport(const glm::uvec2& size, const glm::uvec2& offset = {0, 0}) const;
    std::pair<glm::uvec2, glm::uvec2> getViewport() const;

    // Scissor
    void enableScissor() const;
    void disableScissor() const;
    void setScissor(const glm::uvec2& size, const glm::uvec2& offset) const;

    // Depth Test
    void enableDepthTesting() const;
    void disableDepthTesting() const;
    void setDepthFunction(DepthFunction fun) const;

    // Stencil Test
    void enableStencilTest() const;
    void disableStencilTest() const;
    void setStencilFunction(StencilFunction fun, int ref, unsigned int mask) const;
    void setStencilOp(StencilOp sfail, StencilOp dpfail, StencilOp dppass) const;
    void setStencilMask(unsigned int mask) const;

    // Blend
    void enableBlend() const;
    void disableBlend() const;
    void setBlendFunction(BlendFactor sfactor, BlendFactor dfactor) const;

    // Face Culling
    void enableFaceCulling() const;
    void disableFaceCulling() const;
    void setCullMode(CullMode mode) const;
    void setFrontFace(FrontFace face) const;

    // Clear
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

    // Uniform Buffer
    RendererId createUniformBuffer(size_t size, BufferUsage usage) const;

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

    // Shader Uniform Block
    RendererId getUniformBlockId(RendererId id, const std::string& name);
    void setUniformBlockBindingPoint(
        RendererId programId, RendererId uniformBlockId, size_t bindingPoint);

    // Texture
    RendererId createTexture(int width, int height) const;
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

    // Cube Map
    RendererId createCubeMap(std::vector<SDL_Surface*> surfaces) const;
    void destroyCubeMap(RendererId id) const;

    void bindCubeMap(RendererId id) const;
    void unbindCubeMap() const;

    // Drawing
    void setPolygonMode(PolygonMode mode) const;

    void drawVertices(DrawMode mode, uint32_t first, size_t count);
    void drawIndexed(DrawMode mode, size_t count, IndexType type, const void* offset) const;

    // Frame Buffer
    RendererId createFrameBuffer() const;
    void destroyFrameBuffer(RendererId id) const;

    void bindFrameBuffer(RendererId id) const;
    void bindFrameBufferRead(RendererId id) const;
    void bindFrameBufferWrite(RendererId id) const;

    void unbindFrameBuffer() const;
    void unbindFrameBufferRead() const;
    void unbindFrameBufferWrite() const;

    void setFrameBufferTexture(RendererId id) const;
    void setFrameBufferRenderBuffer(RendererId id) const;

    bool isFrameBufferComplete() const;

    // Render Buffer
    RendererId createRenderBuffer(int width, int height) const;
    void destroyRenderBuffer(RendererId id) const;

    void bindRenderBuffer(RendererId id) const;
    void unbindRenderBuffer() const;
};

} // namespace Runic
