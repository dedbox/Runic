#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/Shader.hpp"

namespace Runic
{

class ShaderProgram
{
public:
    static std::shared_ptr<ShaderProgram> Create(
        GraphicsContext* context, Shader& vertexShader, Shader& fragmentShader);

    ~ShaderProgram();

    // allow moving
    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    // prevent copying
    ShaderProgram(const ShaderProgram&)            = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void bind() const;
    void unbind() const;

    void setUniform(const std::string& name, bool value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, const glm::ivec2& value) const;
    void setUniform(const std::string& name, const glm::ivec3& value) const;
    void setUniform(const std::string& name, const glm::ivec4& value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, const glm::vec2& value) const;
    void setUniform(const std::string& name, const glm::vec3& value) const;
    void setUniform(const std::string& name, const glm::vec4& value) const;
    void setUniform(const std::string& name, const glm::mat3& value) const;
    void setUniform(const std::string& name, const glm::mat4& value) const;

private:
    GraphicsContext* _context;
    RendererId _id;
    mutable std::unordered_map<std::string, RendererId> _uniformLocations;

    // hide constructor
    ShaderProgram(GraphicsContext* context, RendererId id);

    RendererId getUniformLocation(const std::string& name) const;
};

} // namespace Runic
