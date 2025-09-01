#include "Shader.hpp"

namespace Runic
{

Shader::Shader(GraphicsContext* context, RendererId id, const std::string& source)
    : _context(context)
    , _id(id)
{
    if (!_context->compileShader(_id, source.c_str()))
        Core::Error(_context->getShaderInfoLog(_id));
}

Shader::~Shader()
{
    _context->destroyShader(_id);
}

Shader::Shader(Shader&& other) noexcept
    : _context(std::exchange(other._context, nullptr))
    , _id(std::exchange(other._id, 0))
{
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        _context->destroyShader(_id);
        _context = std::exchange(other._context, nullptr);
        _id      = std::exchange(other._id, 0);
    }
    return *this;
}

} // namespace Runic
