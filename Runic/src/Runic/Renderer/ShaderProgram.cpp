#include "ShaderProgram.hpp"

namespace Runic
{

ShaderProgram::ShaderProgram(
    GraphicsContext* gc, RendererId id, const Shader& vertexShader, const Shader& fragmentShader)
    : _gc(gc)
    , _id(id)
{
    _gc->attachShader(_id, vertexShader.getId());
    _gc->attachShader(_id, fragmentShader.getId());

    if (!_gc->linkShaderProgram(_id))
    {
        Core::Error(_gc->getShaderProgramInfoLog(_id));
        _gc->deleteShaderProgram(_id);
        _gc->deleteShader(fragmentShader.getId());
        _gc->deleteShader(vertexShader.getId());
        Core::Assert(false, "shader program linking failed");
    }

    _gc->detachShader(_id, vertexShader.getId());
    _gc->detachShader(_id, fragmentShader.getId());
}

ShaderProgram::~ShaderProgram()
{
    _gc->deleteShaderProgram(_id);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : _gc(std::exchange(other._gc, nullptr))
    , _id(std::exchange(other._id, 0))
{
}

// TODO make deletable RendererId's optional
ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this != &other)
    {
        _gc->deleteShaderProgram(_id);
        _gc = std::exchange(other._gc, nullptr);
        _id = std::exchange(other._id, 0);
    }
    return *this;
}

void ShaderProgram::use() const
{
    _gc->useShaderProgram(_id);
}

void ShaderProgram::setUniform(const std::string& name, bool value) const
{
    _gc->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, int value) const
{
    _gc->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::ivec2& value) const
{
    _gc->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::ivec3& value) const
{
    _gc->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::ivec4& value) const
{
    _gc->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, float value) const
{
    _gc->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec2& value) const
{
    _gc->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) const
{
    _gc->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec4& value) const
{
    _gc->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat3& value) const
{
    _gc->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& value) const
{
    _gc->setUniform(getUniformLocation(name), value);
}

RendererId ShaderProgram::getUniformLocation(const std::string& name) const
{
    if (_uniformLocations.count(name)) return _uniformLocations[name];

    RendererId location     = _gc->getUniformLocation(_id, name);
    _uniformLocations[name] = location;
    return location;
}

} // namespace Runic
