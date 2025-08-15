#include "pch.hpp"

#include "glad/gl.h"

#include "OpenGLContext.hpp"

namespace Runic
{
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : _windowHandle(windowHandle)
{
    RUNIC_CORE_ASSERT(_windowHandle, "Window handle is null!");
}

void OpenGLContext::init()
{
    glfwMakeContextCurrent(_windowHandle);

    const int version = gladLoadGL(glfwGetProcAddress);
    RUNIC_CORE_ASSERT(version, "Could not load glad!");
    RUNIC_CORE_INFO("Loaded OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    RUNIC_CORE_INFO("OpenGL info:");
    RUNIC_CORE_INFO("    Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    RUNIC_CORE_INFO("    Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    RUNIC_CORE_INFO("    Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
}

void OpenGLContext::swapBuffers()
{
    glfwSwapBuffers(_windowHandle);
}
} // Runic
