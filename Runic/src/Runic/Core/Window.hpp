#pragma once

#include "SDL3/SDL_video.h"
#include "glm/ext/vector_int2.hpp"

namespace Runic
{

constexpr uint32_t DEFAULT_WIDTH{1280};
constexpr uint32_t DEFAULT_HEIGHT{720};

struct WindowData
{
    std::string title = "Runic Engine";
    uint32_t width    = DEFAULT_WIDTH;
    uint32_t height   = DEFAULT_HEIGHT;
};

class Window
{
public:
    explicit Window(const WindowData& data);

    SDL_Window* getNative() const { return _native; }

    void show() const;
    void hide() const;

    const glm::ivec2 getSize() const;

private:
    SDL_Window* _native = nullptr;
};

} // namespace Runic
