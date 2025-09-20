#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{

class RenderBuffer
{
private:
    GraphicsContext* _context;
    RendererId _id;

    explicit RenderBuffer(GraphicsContext* context, RendererId id);

public:
    static std::unique_ptr<RenderBuffer> Create(GraphicsContext* context, int width, int height);

    ~RenderBuffer();

    // allow moving
    RenderBuffer(RenderBuffer&& other) noexcept;
    RenderBuffer& operator=(RenderBuffer&& other) noexcept;

    // prevent copying
    RenderBuffer(const RenderBuffer&)            = delete;
    RenderBuffer& operator=(const RenderBuffer&) = delete;

    RendererId getId() const { return _id; }

    void bind() const;
    void unbind() const;
};

} // namespace Runic
