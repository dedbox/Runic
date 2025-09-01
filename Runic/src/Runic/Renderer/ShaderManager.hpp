#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"

namespace Runic
{

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
    static void Load(
        GraphicsContext* context, const std::string& vertexName, const std::string& fragmentName);

    /** Retrieves ShaderProgram objects from the shader cache.
     *
     * This method will Load the ShaderProgram object if it hasn't already been loaded.
     */
    static std::shared_ptr<ShaderProgram> Find(
        GraphicsContext* context, const std::string& vertexName, const std::string& fragmentName);

private:
    static std::map<Strings, std::shared_ptr<ShaderProgram>> _shaders;
};

} // namespace Runic
