#include "Shader.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{

std::unique_ptr<Shader> Shader::Create(
    GraphicsContext* context, const std::string& source, ShaderType type)
{
    RendererId id = context->createShader(type);
    return std::unique_ptr<Shader>(new Shader(context, id, source));
}

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

void Shader::attach(RendererId programId) const
{
    _context->attachShader(programId, _id);
}

void Shader::detach(RendererId programId) const
{
    _context->detachShader(programId, _id);
}

void Shader::destroy()
{
    _context->destroyShader(_id);
    _context = nullptr;
    _id      = 0;
}

} // namespace Runic
