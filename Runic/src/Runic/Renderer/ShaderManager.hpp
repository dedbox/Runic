#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"

namespace Runic
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

using Strings = std::pair<std::string, std::string>;

class ShaderManager
{
public:
    /** Loads and caches ShaderProgram objects from GLSL source files.
     *
     * Finds shader source files in the "shaders/" directory with the appropriate ".vert" or ".frag"
     * file extension.
     *
     * For example, if vertexName is "Position" and fragmentName is "FlatUniform", the files
     * "shaders/Position.vert" and "shaders/FlatUniform.frag" are compiled and linked into a
     * ShaderProgram object.
     */
    template <typename... Layouts>
    static void Load(
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

        ShaderManager::_shaders[key] =
            ShaderProgram<Layouts...>::Create(context, *vertexShader, *fragmentShader);
    }

    /** Retrieves ShaderProgram objects from the shader cache.
     *
     * This method will Load the ShaderProgram object if it hasn't already been loaded.
     */
    template <typename... Layouts>
    static std::shared_ptr<ShaderProgram<Layouts...>> Find(
        GraphicsContext* context, const std::string& vertexName, const std::string& fragmentName)
    {
        const Strings key(vertexName, fragmentName);

        if (!_shaders.contains(key))
            Load<Layouts...>(context, vertexName, fragmentName);

        return std::dynamic_pointer_cast<ShaderProgram<Layouts...>>(_shaders[key]);
    }

private:
    static std::map<Strings, std::shared_ptr<ShaderProgramBase>> _shaders;
};

} // namespace Runic
