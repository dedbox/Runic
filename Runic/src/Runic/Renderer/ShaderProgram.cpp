#include "ShaderProgram.hpp"

namespace Runic
{

ShaderProgram::ShaderProgram(
    GraphicsContext* context,
    RendererId id,
    const Shader& vertexShader,
    const Shader& fragmentShader)
    : _context(context)
    , _id(id)
{
    _context->attachShader(_id, vertexShader.getId());
    _context->attachShader(_id, fragmentShader.getId());

    if (!_context->linkShaderProgram(_id))
    {
        Core::Error(_context->getShaderProgramInfoLog(_id));
        _context->destroyShaderProgram(_id);
        _context->destroyShader(fragmentShader.getId());
        _context->destroyShader(vertexShader.getId());
        Core::Assert(false, "shader program linking failed");
    }

    _context->detachShader(_id, vertexShader.getId());
    _context->detachShader(_id, fragmentShader.getId());
}

ShaderProgram::~ShaderProgram()
{
    _context->destroyShaderProgram(_id);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : _context(std::exchange(other._context, nullptr))
    , _id(std::exchange(other._id, 0))
{
}

// TODO make deletable RendererId's optional
ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this != &other)
    {
        _context->destroyShaderProgram(_id);
        _context = std::exchange(other._context, nullptr);
        _id      = std::exchange(other._id, 0);
    }
    return *this;
}

void ShaderProgram::bind() const
{
    _context->useShaderProgram(_id);
}

void ShaderProgram::unbind() const
{
    _context->useShaderProgram(0);
}

void ShaderProgram::setUniform(const std::string& name, bool value) const
{
    _context->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, int value) const
{
    _context->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::ivec2& value) const
{
    _context->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::ivec3& value) const
{
    _context->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::ivec4& value) const
{
    _context->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, float value) const
{
    _context->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec2& value) const
{
    _context->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) const
{
    _context->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec4& value) const
{
    _context->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat3& value) const
{
    _context->setUniform(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& value) const
{
    _context->setUniform(getUniformLocation(name), value);
}

RendererId ShaderProgram::getUniformLocation(const std::string& name) const
{
    if (_uniformLocations.count(name))
        return _uniformLocations[name];

    RendererId location     = _context->getUniformLocation(_id, name);
    _uniformLocations[name] = location;
    return location;
}

} // namespace Runic
