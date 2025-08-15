#pragma once

#include "GLFW/glfw3.h"

#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{
class OpenGLContext final : public GraphicsContext
{
public:
    explicit OpenGLContext(GLFWwindow* windowHandle);

    void init() override;

    void swapBuffers() override;

private:
    GLFWwindow* _windowHandle;
};
} // Runic
