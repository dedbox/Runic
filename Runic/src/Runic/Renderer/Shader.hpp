#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{

class Shader
{
public:
    static std::unique_ptr<Shader> Create(
        GraphicsContext* context, const std::string& source, ShaderType type);

    ~Shader();

    // allow moving
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    // prevent copying
    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    void attach(RendererId programId) const;
    void detach(RendererId programId) const;

    void destroy();

private:
    GraphicsContext* _context;
    RendererId _id;

    // hide constructor
    Shader(GraphicsContext* context, RendererId id, const std::string& source);
};

} // namespace Runic
