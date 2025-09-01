#include "Runic/Renderer/ShaderManager.hpp"

namespace Runic
{

namespace
{

static std::string LoadFile(const std::string& path)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        file.open(path);

        std::stringstream stream;
        stream << file.rdbuf();
        file.close();

        return stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        Core::Error("File `{}' could not be loaded: {}", path, e.what());
        throw;
    }
}

} // namespace

std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> ShaderManager::_shaders;

void ShaderManager::LoadShader(
    GraphicsContext* context, const std::string& vertexName, const std::string& fragmentName)
{

    const std::string shaderName = vertexName + "-" + fragmentName;

    if (ShaderManager::_shaders.count(shaderName))
    {
        Core::Warn("Multiple attempts to load shader `{}'", shaderName);
        return;
    }

    const std::string vertexSource   = LoadFile(std::format("shaders/{}.vert", vertexName));
    const std::string fragmentSource = LoadFile(std::format("shaders/{}.frag", fragmentName));

    const auto vertexShader   = Shader::Create(context, vertexSource, ShaderType::Vertex);
    const auto fragmentShader = Shader::Create(context, fragmentSource, ShaderType::Fragment);

    ShaderManager::_shaders[shaderName] =
        ShaderProgram::Create(context, *vertexShader, *fragmentShader);
}

std::shared_ptr<ShaderProgram> ShaderManager::getShader(const std::string& name)
{
    Core::Assert(
        ShaderManager::_shaders.count(name), "Attempted to get unloaded shader `{}'", name);

    return ShaderManager::_shaders[name];
}

} // namespace Runic
