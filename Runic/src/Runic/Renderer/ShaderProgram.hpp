#pragma once

#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/ext/vector_int2.hpp"
#include "glm/ext/vector_int3.hpp"
#include "glm/ext/vector_int4.hpp"

#include "GraphicsContext.hpp"
#include "Shader.hpp"

namespace Runic
{

class ShaderProgram
{
public:
    ShaderProgram(
        GraphicsContext* gc,
        RendererId id,
        const Shader& vertexShader,
        const Shader& fragmentShader);

    ~ShaderProgram();

    // allow moving
    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    // prevent copying
    ShaderProgram(const ShaderProgram&)            = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void use() const;

    void setUniformBool(const std::string& name, bool value) const;
    void setUniformInt(const std::string& name, int value) const;
    void setUniformInt2(const std::string& name, const glm::ivec2& value) const;
    void setUniformInt3(const std::string& name, const glm::ivec3& value) const;
    void setUniformInt4(const std::string& name, const glm::ivec4& value) const;
    void setUniformFloat(const std::string& name, float value) const;
    void setUniformFloat2(const std::string& name, const glm::vec2& value) const;
    void setUniformFloat3(const std::string& name, const glm::vec3& value) const;
    void setUniformFloat4(const std::string& name, const glm::vec4& value) const;
    void setUniformMat3(const std::string& name, const glm::mat3& value) const;
    void setUniformMat4(const std::string& name, const glm::mat4& value) const;

private:
    GraphicsContext* _gc;
    RendererId _id;
    mutable std::unordered_map<std::string, RendererId> _uniformLocations;

    RendererId getUniformLocation(const std::string& name) const;
};

} // namespace Runic
