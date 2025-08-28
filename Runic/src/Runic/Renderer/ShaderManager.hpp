#pragma once

#include "Renderer.hpp"
#include "ShaderProgram.hpp"

namespace Runic
{

class ShaderManager
{
public:
    static void LoadShader(
        const Renderer& renderer,
        const std::string& name,
        const std::string& vertexPath,
        const std::string& fragmentPath);

    static std::shared_ptr<ShaderProgram> getShader(const std::string& name);

private:
    static std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> _shaders;
};

} // namespace Runic
