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

std::map<Strings, std::shared_ptr<ShaderProgramBase>> ShaderManager::_shaders;

void ShaderManager::Load(
    GraphicsContext* context, const std::string& vertexName, const std::string& fragmentName)
{
    const Strings key(vertexName, fragmentName);

    if (ShaderManager::_shaders.contains(key))
    {
        Core::Warn("Multiple attempts to load shader `{{{}, {}}}'", vertexName, fragmentName);
        return;
    }

    const std::string vertexSource   = LoadFile(std::format("shaders/{}.vert", vertexName));
    const std::string fragmentSource = LoadFile(std::format("shaders/{}.frag", fragmentName));

    const auto vertexShader   = Shader::Create(context, vertexSource, ShaderType::Vertex);
    const auto fragmentShader = Shader::Create(context, fragmentSource, ShaderType::Fragment);

    ShaderManager::_shaders[key] = ShaderProgram::Create(context, *vertexShader, *fragmentShader);
}

std::shared_ptr<ShaderProgram> ShaderManager::Find(
    GraphicsContext* context, const std::string& vertexName, const std::string& fragmentName)
{
    const Strings key(vertexName, fragmentName);

    if (!_shaders.contains(key))
        Load(context, vertexName, fragmentName);

    return std::dynamic_pointer_cast<ShaderProgram>(_shaders[key]);
}

} // namespace Runic
