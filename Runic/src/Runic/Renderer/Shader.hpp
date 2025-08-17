#pragma once

#include "pch.hpp"
#include "glm/glm.hpp"

namespace Runic
{
class Shader
{
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);

    ~Shader();

    void bind() const;

    static void unbind();

    void uploadUniformFloat4(const std::string& name, const glm::vec4& vector) const;

    void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

private:
    uint32_t _rendererID;
};
}
