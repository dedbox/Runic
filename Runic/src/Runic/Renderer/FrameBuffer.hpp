#pragma once

#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/RenderBuffer.hpp"
#include "Runic/Renderer/Texture.hpp"

namespace Runic
{

class FrameBuffer
{
private:
    GraphicsContext* _context;
    RendererId _id;
    std::shared_ptr<Texture> _texture;

    explicit FrameBuffer(GraphicsContext* context, RendererId id, std::shared_ptr<Texture> texture);

public:
    static std::unique_ptr<FrameBuffer> Create(GraphicsContext* context, int width, int height);

    ~FrameBuffer();

    // allow moving
    FrameBuffer(FrameBuffer&& other) noexcept;
    FrameBuffer& operator=(FrameBuffer&& other) noexcept;

    // prevent copying
    FrameBuffer(const FrameBuffer&)            = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;

    void bind() const;
    void unbind() const;

    std::shared_ptr<Texture> getTexture() { return _texture; }

    void setRenderBuffer(std::unique_ptr<RenderBuffer> renderBuffer);

private:
    std::unique_ptr<RenderBuffer> _renderBuffer;
};

} // namespace Runic
