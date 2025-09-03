#pragma once

#include "Runic/Core/Layer.hpp"
#include "Runic/Core/Window.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{

class GuiLayer : public Layer
{
public:
    GuiLayer(Window* window, GraphicsContext* context);

    void attach() override;
    void detach() override;
    void render() override;

    virtual void renderGui() {}

protected:
    Window* getWindow() { return _window; }
    GraphicsContext* getGraphicsContext() { return _context; }

private:
    Window* _window;
    GraphicsContext* _context;
};

} // namespace Runic
