#pragma once

#include "GLFW/glfw3.h"

#include "Runic/Renderer/GraphicsContext.hpp"

namespace Runic
{
class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(GLFWwindow* windowHandle);

    void init() override;

    void swapBuffers() override;

private:
    GLFWwindow* _windowHandle;
};
} // Runic
