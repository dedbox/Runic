#include "ImGuiLayer.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

#include "Runic/Application.hpp"

extern ImGuiKey ImGui_ImplGlfw_KeyToImGuiKey(int keycode, int scancode);

namespace Runic
{
ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer") {}

void ImGuiLayer::onAttach()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    const float mainScale{ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor())};

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;

    const Application& app{Application::get()};
    const auto window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::onDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::onUpdate()
{
    const Application& app = Application::get();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(app.getWindow().getWidth()),
                            static_cast<float>(app.getWindow().getHeight()));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool showDemo = true;
    ImGui::ShowDemoWindow(&showDemo);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // Runic
