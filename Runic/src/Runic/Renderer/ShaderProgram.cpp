#include "Runic/Renderer/ShaderProgram.hpp"

namespace Runic
{
std::shared_ptr<ShaderProgram> ShaderProgram::Create(
    GraphicsContext* context, Shader& vertexShader, Shader& fragmentShader)
{
    RendererId id = context->createShaderProgram();

    vertexShader.attach(id);
    fragmentShader.attach(id);

    if (!context->linkShaderProgram(id))
    {
        Core::Error(context->getShaderProgramInfoLog(id));
        Core::Assert(false, "shader program linking failed");
        context->destroyShaderProgram(id);
        fragmentShader.destroy();
        vertexShader.destroy();
    }

    vertexShader.detach(id);
    fragmentShader.detach(id);

    return std::unique_ptr<ShaderProgram>(new ShaderProgram(context, id));
}

ShaderProgram::ShaderProgram(GraphicsContext* context, RendererId id)
    : _context(context)
    , _id(id)
{
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
