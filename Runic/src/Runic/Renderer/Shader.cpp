#include "Shader.hpp"

namespace Runic
{

Shader::Shader(GraphicsContext* gc, RendererId id, const std::string& source)
    : _gc(gc)
    , _id(id)
{
    if (!_gc->compileShader(_id, source.c_str()))
        Core::Error(_gc->getShaderInfoLog(_id));
}

Shader::~Shader()
{
    _gc->destroyShader(_id);
}

Shader::Shader(Shader&& other) noexcept
    : _gc(std::exchange(other._gc, nullptr))
    , _id(std::exchange(other._id, 0))
{
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        _gc->destroyShader(_id);
        _gc = std::exchange(other._gc, nullptr);
        _id = std::exchange(other._id, 0);
    }
    return *this;
}

} // namespace Runic
