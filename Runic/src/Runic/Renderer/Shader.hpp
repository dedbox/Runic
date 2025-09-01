#pragma once

#include "GraphicsContext.hpp"

namespace Runic
{

class Shader
{
public:
    Shader(GraphicsContext* context, RendererId id, const std::string& source);

    ~Shader();

    // allow moving
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    // prevent copying
    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    RendererId getId() const { return _id; }

private:
    GraphicsContext* _context;
    RendererId _id;
};

} // namespace Runic
