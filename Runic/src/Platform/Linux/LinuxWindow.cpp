#include "pch.hpp"

#include "Platform/Linux/LinuxWindow.hpp"
#include "Platform/OpenGL/OpenGLContext.hpp"
#include "Runic/Event/ApplicationEvent.hpp"
#include "Runic/Event/KeyEvent.hpp"
#include "Runic/Event/MouseEvent.hpp"

namespace Runic
{
static bool s_GLFWInitialized = false;

static void handleGlfwError(int error, const char* description)
{
    RUNIC_CORE_ERROR("GLFW ERROR ({}): {}", error, description);
}

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
        // ReSharper disable once CppDFAUnusedValue
        int success = glfwInit();
        RUNIC_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(handleGlfwError);
        s_GLFWInitialized = true;
    }

    _window = glfwCreateWindow(_data.width, _data.height, _data.title.c_str(), nullptr, nullptr);

    _context = new OpenGLContext(_window);
    _context->init();;

    glfwSetWindowUserPointer(_window, &_data);
    setVSync(true);

    // events
    glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, const int width, const int height) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

        if (data.width != width || data.height != height) {
            WindowResizeEvent event(width, height);
            data.width = width;
            data.height = height;
            data.eventCallback(event);
        }
    });

    glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
        const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        WindowCloseEvent event;
        data.eventCallback(event);
    });

    glfwSetKeyCallback(
        _window, [](GLFWwindow* window, const int keyCode, const int scanCode, const int action, const int /*mods*/) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(keyCode, scanCode, false);
                    data.eventCallback(event);
                    break;
                }

                case GLFW_RELEASE: {
                    KeyReleasedEvent event(keyCode, scanCode);
                    data.eventCallback(event);
                    break;
                }

                case GLFW_REPEAT: {
                    KeyPressedEvent event(keyCode, scanCode, true);
                    data.eventCallback(event);
                    break;
                }

                default:
                    RUNIC_CORE_ASSERT(false, "Unknown keyCode action!");
                    break;
            }
        });

    glfwSetCharCallback(
        _window, [](GLFWwindow* window, const unsigned int keyCode) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(keyCode);
            data.eventCallback(event);
        });

    glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, const int button, const int action, const int /*mods*/) {
        const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                data.eventCallback(event);
                break;
            }

            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                data.eventCallback(event);
                break;
            }

            default:
                RUNIC_CORE_ASSERT(false, "Unknown mouse action!");
                break;
        }
    });

    glfwSetScrollCallback(_window, [](GLFWwindow* window, const double xOffset, const double yOffset) {
        const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
        data.eventCallback(event);
    });

    glfwSetCursorPosCallback(_window, [](GLFWwindow* window, const double xPos, const double yPos) {
        const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
        data.eventCallback(event);
    });
}

void LinuxWindow::shutdown() const
{
    glfwDestroyWindow(_window);
    glfwTerminate();
    s_GLFWInitialized = false;
}

void LinuxWindow::onUpdate()
{
    glfwPollEvents();
    _context->swapBuffers();
}

void LinuxWindow::setVSync(const bool enabled)
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
