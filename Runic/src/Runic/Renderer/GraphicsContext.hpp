#pragma once

#include "SDL3/SDL_video.h"
#include "glm/ext/vector_float4.hpp"
#include "glm/ext/vector_int2.hpp"

#include "Runic/Core/Window.hpp"

namespace Runic
{

class GraphicsContext
{
public:
    explicit GraphicsContext(Window* window);

    void setViewport(const glm::ivec2& size, const glm::ivec2& offset = {0, 0});

    void setClearColor(const glm::vec4& color);
    void clear() const;
    void swapBuffers() const;

private:
    Window* _window;
    SDL_GLContext _context;
};

} // namespace Runic
