#include "pch.hpp"
#include "Platform/Linux/LinuxInput.hpp"

#include "GLFW/glfw3.h"
#include "Runic/Application.hpp"

namespace Runic
{
Input* Input::s_instance{new LinuxInput};

bool LinuxInput::isKeyPressedImpl(const int keyCode)
{
    const auto window{static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow())};
    const auto state{glfwGetKey(window, keyCode)};

    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool LinuxInput::isMouseButtonPressedImpl(const int button)
{
    const auto window{static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow())};
    const auto state{glfwGetMouseButton(window, button)};

    return state == GLFW_PRESS;
}

std::pair<float, float> LinuxInput::getMousePositionImpl()
{
    const auto window{static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow())};
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    return {static_cast<float>(xPos), static_cast<float>(yPos)};
}
} // Runic
