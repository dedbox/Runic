#pragma once

#include "SDL3/SDL_video.h"

namespace Runic
{

constexpr uint32_t DEFAULT_WIDTH{1280};
constexpr uint32_t DEFAULT_HEIGHT{720};

struct WindowData {
    std::string title{"Runic Engine"};
    uint32_t width{DEFAULT_WIDTH};
    uint32_t height{DEFAULT_HEIGHT};
};

class Window
{
public:
    explicit Window(WindowData data);

    [[nodiscard]] const WindowData& getData() const;

    void show() const;
    void hide() const;

private:
    WindowData _data;
    SDL_Window* _native;
};

} // namespace Runic
