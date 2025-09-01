#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"

namespace Runic
{

class ShaderManager
{
public:
    /** Loads and caches ShaderProgram objects from GLSL source files.
     *
     * Finds shader source files in the "shaders/" directory with the appropriate ".vert" or ".frag"
     * file extension. The corresponding ShaderProgram object is stored in memory under the name
     * "vertexName-FragmentName".
     *
     * For example, if vertexName is "Position" and fragmentName is "FlatUniform", the files
     * "shaders/Position.vert" and "shaders/FlatUniform.frag" are compiled and linked into a
     * ShaderProgram object stored in memory under the name "Position-FlatUniform".
     */
    static void LoadShader(
        GraphicsContext* context, const std::string& vertexName, const std::string& fragmentName);

    static std::shared_ptr<ShaderProgram> getShader(const std::string& name);

private:
    static std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> _shaders;
};

} // namespace Runic
