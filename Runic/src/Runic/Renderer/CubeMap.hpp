#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{

class CubeMap
{
private:
    GraphicsContext* _context;
    RendererId _id;

    CubeMap(GraphicsContext* context, RendererId id);

public:
    static std::shared_ptr<CubeMap> Create(
        GraphicsContext* context, const std::vector<SDL_Surface*>& surfaces);

    ~CubeMap();

    // allow moving
    CubeMap(CubeMap&& other) noexcept;
    CubeMap& operator=(CubeMap&& other) noexcept;

    // prevent copying
    CubeMap(const CubeMap&)            = delete;
    CubeMap& operator=(const CubeMap&) = delete;

    void bind() const;
    void unbind() const;
};

} // namespace Runic
