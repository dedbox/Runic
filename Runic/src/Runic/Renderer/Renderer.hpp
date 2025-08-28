#pragma once

#include "glm/ext/vector_float4.hpp"
#include "glm/ext/vector_int2.hpp"

#include "Buffer.hpp"
#include "GraphicsContext.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "VertexArray.hpp"

namespace Runic
{

class Renderer
{
public:
    explicit Renderer(std::unique_ptr<GraphicsContext>&& gc);

    GraphicsContext& getGraphicsContext() { return *_gc; }

    void beginFrame();
    void endFrame();

    void setViewport(const glm::ivec2& size, const glm::ivec2& offset = {0, 0}) const;
    void setClearColor(const glm::vec4& color) const;
    void clear() const;

    [[nodiscard]] std::unique_ptr<VertexArray> createVertexArray() const;

    std::unique_ptr<VertexBuffer>
    createVertexBuffer(const void* data, size_t size, BufferUsage usage) const;

    std::unique_ptr<IndexBuffer> createIndexBuffer(
        const void* data, size_t count, IndexType type, IndexMode mode, BufferUsage usage) const;

    [[nodiscard]] std::unique_ptr<Shader>
    createShader(const std::string& source, ShaderType type) const;

    [[nodiscard]] std::shared_ptr<ShaderProgram>
    createShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) const;

private:
    std::unique_ptr<GraphicsContext> _gc;
};

} // namespace Runic
