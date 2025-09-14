#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/Shader.hpp"

namespace Runic
{

class ShaderProgramBase
{
public:
    ShaderProgramBase() = default;

    // allow moving
    ShaderProgramBase(ShaderProgramBase&&)            = default;
    ShaderProgramBase& operator=(ShaderProgramBase&&) = default;

    // prevent copying
    ShaderProgramBase(const ShaderProgramBase&)            = delete;
    ShaderProgramBase& operator=(const ShaderProgramBase&) = delete;

    virtual ~ShaderProgramBase() = default;

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual void setUniform(const std::string& name, bool value) const       = 0;
    virtual void setUniform(const std::string& name, int value) const        = 0;
    virtual void setUniform(const std::string& name, glm::ivec2 value) const = 0;
    virtual void setUniform(const std::string& name, glm::ivec3 value) const = 0;
    virtual void setUniform(const std::string& name, glm::ivec4 value) const = 0;
    virtual void setUniform(const std::string& name, float value) const      = 0;
    virtual void setUniform(const std::string& name, glm::vec2 value) const  = 0;
    virtual void setUniform(const std::string& name, glm::vec3 value) const  = 0;
    virtual void setUniform(const std::string& name, glm::vec4 value) const  = 0;
    virtual void setUniform(const std::string& name, glm::mat3 value) const  = 0;
    virtual void setUniform(const std::string& name, glm::mat4 value) const  = 0;
};

class ShaderProgram : public ShaderProgramBase
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

    ~ShaderProgram() override { _context->destroyShaderProgram(_id); }

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

    void bind() const override { _context->useShaderProgram(_id); }
    void unbind() const override { _context->useShaderProgram(0); }

    void setUniform(const std::string& name, auto value) const
    {
        _context->setUniform(getUniformLocation(name), value);
    }

    void setUniform(const std::string& name, bool value) const override
    {
        _context->setUniform(getUniformLocation(name), value);
    }

    void setUniform(const std::string& name, int value) const override
    {
        _context->setUniform(getUniformLocation(name), value);
    }

    void setUniform(const std::string& name, glm::ivec2 value) const override
    {
        _context->setUniform(getUniformLocation(name), value);
    }

    void setUniform(const std::string& name, glm::ivec3 value) const override
    {
        _context->setUniform(getUniformLocation(name), value);
    }

    void setUniform(const std::string& name, glm::ivec4 value) const override
    {
        _context->setUniform(getUniformLocation(name), value);
    }

    void setUniform(const std::string& name, float value) const override
    {
        _context->setUniform(getUniformLocation(name), value);
    }

    void setUniform(const std::string& name, glm::vec2 value) const override
    {
        _context->setUniform(getUniformLocation(name), value);
    }

    void setUniform(const std::string& name, glm::vec3 value) const override
    {
        _context->setUniform(getUniformLocation(name), value);
    }

    void setUniform(const std::string& name, glm::vec4 value) const override
    {
        _context->setUniform(getUniformLocation(name), value);
    }

    void setUniform(const std::string& name, glm::mat3 value) const override
    {
        _context->setUniform(getUniformLocation(name), value);
    }

    void setUniform(const std::string& name, glm::mat4 value) const override
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
