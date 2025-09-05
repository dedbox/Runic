#pragma once

#include "SDL3/SDL_video.h"
#include "glm/ext/vector_int2.hpp"
#include "glm/ext/vector_uint2.hpp"

#include "Runic/Renderer/Color.hpp"

namespace Runic
{

constexpr uint32_t DEFAULT_WIDTH{1280};
constexpr uint32_t DEFAULT_HEIGHT{720};

// Resize Policy -----------------------------------------------------------------------------------

struct AdaptiveResize
{
};

struct FixedAspect
{
    float aspectRatio;
    color frameColor = Color::Black;
};

using ResizePolicy = std::variant<AdaptiveResize, FixedAspect>;

// -------------------------------------------------------------------------------------------------

struct WindowData
{
    std::string title         = "Runic Engine";
    uint32_t width            = DEFAULT_WIDTH;
    uint32_t height           = DEFAULT_HEIGHT;
    ResizePolicy resizePolicy = AdaptiveResize();
};

class Window
{
public:
    explicit Window(const WindowData& data);

    SDL_Window* getNative() const { return _native; }

    const ResizePolicy& getResizePolicy() const { return _resizePolicy; }

    void show() const;
    void hide() const;

    const glm::ivec2 getSize() const;
    void setSize(const glm::uvec2& size);

    void setViewport(const glm::uvec2& size, const glm::uvec2& offset = {0, 0})
    {
        _viewportSize   = size;
        _viewportOffset = offset;
    }

    std::pair<glm::uvec2, glm::uvec2> getViewport() const
    {
        return {_viewportSize, _viewportOffset};
    }

    void captureMouse() const;
    void releaseMouse() const;
    bool isMouseCaptured() const;

private:
    SDL_Window* _native = nullptr;

    ResizePolicy _resizePolicy;
    glm::uvec2 _viewportSize;
    glm::uvec2 _viewportOffset;
};

} // namespace Runic
