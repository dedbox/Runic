#include "Platform/Linux/LinuxWindow.hpp"

#include "pch.hpp"

namespace Runic
{
static bool s_GLFWInitialized = false;

Window* Window::create(const WindowProps& props)
{
    return new LinuxWindow(props);
}

LinuxWindow::LinuxWindow(const WindowProps& props)
{
    init(props);
}

LinuxWindow::~LinuxWindow()
{
    shutdown();
}

void LinuxWindow::init(const WindowProps& props)
{
    RUNIC_CORE_INFO("Creating window \"{}\" ({}x{})", props.title, props.width, props.height);

    _data.title = props.title;
    _data.width = props.width;
    _data.height = props.height;

    if (!s_GLFWInitialized) {
        int success = glfwInit();
        RUNIC_CORE_ASSERT(success, "Could not initialize GLFW!");
        s_GLFWInitialized = true;
    }

    _window = glfwCreateWindow(_data.width, _data.height, _data.title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(_window);
    glfwSetWindowUserPointer(_window, &_data);
    setVSync(true);
}

void LinuxWindow::shutdown()
{
    glfwDestroyWindow(_window);
}

void LinuxWindow::onUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(_window);
}

void LinuxWindow::setVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    _data.vsync = enabled;
}

bool LinuxWindow::isVSync() const
{
    return _data.vsync;
}
}
