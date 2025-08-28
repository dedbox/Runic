#include "ShaderManager.hpp"

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
    const Renderer& renderer,
    const std::string& name,
    const std::string& vertexPath,
    const std::string& fragmentPath)
{
    if (ShaderManager::_shaders.count(name))
    {
        Core::Warn("Multiple attempts to load shader `{}'", name);
        return;
    }

    std::string vertexSource   = LoadFile(vertexPath);
    std::string fragmentSource = LoadFile(fragmentPath);

    const auto vertexShader   = renderer.createShader(vertexSource, Runic::ShaderType::Vertex);
    const auto fragmentShader = renderer.createShader(fragmentSource, Runic::ShaderType::Fragment);

    ShaderManager::_shaders[name] = renderer.createShaderProgram(*vertexShader, *fragmentShader);
}

std::shared_ptr<ShaderProgram> ShaderManager::getShader(const std::string& name)
{
    Core::Assert(
        ShaderManager::_shaders.count(name), "Attempted to get unloaded shader `{}'", name);

    return ShaderManager::_shaders[name];
}

} // namespace Runic
