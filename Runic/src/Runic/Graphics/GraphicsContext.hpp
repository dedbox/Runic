#pragma once

#include "SDL3/SDL_video.h"
#include "glm/ext/vector_float4.hpp"

#include "Runic/Core/Window.hpp"

namespace Runic
{

class GraphicsContext
{
public:
    explicit GraphicsContext(Window* window);

    void setClearColor(const glm::vec4& color);
    void clear() const;
    void swapBuffers() const;

private:
    Window* _window;
    SDL_GLContext _context;
};

} // namespace Runic
