#pragma once

#include "GraphicsContext.hpp"

namespace Runic
{

class Shader
{
public:
    Shader(GraphicsContext* gc, RendererId id, const std::string& source);

    ~Shader();

    // allow moving
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    // prevent copying
    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    [[nodiscard]] RendererId getId() const { return _id; }

private:
    GraphicsContext* _gc;
    RendererId _id;
};

} // namespace Runic
