#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/Shader.hpp"

namespace Runic
{

class ShaderProgram
{
private:
    GraphicsContext* _context;
    RendererId _id;

    ShaderProgram(GraphicsContext* context, RendererId id)
        : _context(context)
        , _id(id)
    {
    }

public:
    static std::shared_ptr<ShaderProgram> Create(
        GraphicsContext* context, Shader& vertexShader, Shader& fragmentShader)
    {
        RendererId id = context->createShaderProgram();

        vertexShader.attach(id);
        fragmentShader.attach(id);

        if (!context->linkShaderProgram(id))
        {
            Core::Error("{}", context->getShaderProgramInfoLog(id));
            Core::Assert(false, "shader program linking failed");
            context->destroyShaderProgram(id);
            fragmentShader.destroy();
            vertexShader.destroy();
        }

        vertexShader.detach(id);
        fragmentShader.detach(id);

        return std::unique_ptr<ShaderProgram>(new ShaderProgram(context, id));
    }

    ~ShaderProgram() { _context->destroyShaderProgram(_id); }

    // allow moving
    ShaderProgram(ShaderProgram&& other) noexcept
        : _context(std::exchange(other._context, nullptr))
        , _id(std::exchange(other._id, 0))
    {
    }

    ShaderProgram& operator=(ShaderProgram&& other) noexcept
    {
        if (this != &other)
        {
            _context->destroyShaderProgram(_id);
            _context = std::exchange(other._context, nullptr);
            _id      = std::exchange(other._id, 0);
        }
        return *this;
    }

    // prevent copying
    ShaderProgram(const ShaderProgram&)            = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void bind() const { _context->useShaderProgram(_id); }
    void unbind() const { _context->useShaderProgram(0); }

    void setUniform(const std::string& name, auto value) const
    {
        _context->setUniform(getUniformLocation(name), value);
    }

private:
    mutable std::unordered_map<std::string, RendererId> _uniformLocations;

    RendererId getUniformLocation(const std::string& name) const
    {
        if (_uniformLocations.count(name))
            return _uniformLocations[name];

        RendererId location     = _context->getUniformLocation(_id, name);
        _uniformLocations[name] = location;
        return location;
    }
};

} // namespace Runic
