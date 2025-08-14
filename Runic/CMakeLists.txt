cmake_minimum_required(VERSION 3.31)
project(Runic)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_POSITION_INDEPENDENT_CODE 1)

find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.3)

FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG v2.0.8
        SOURCE_SUBDIR cmake)

FetchContent_MakeAvailable(glad)

glad_add_library(glad_gl_core_46 STATIC LOADER API gl:core=4.6)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git)

FetchContent_MakeAvailable(imgui)

add_library(ImGui STATIC
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imstb_rectpack.h
        ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h)

target_include_directories(ImGui PUBLIC ${imgui_SOURCE_DIR})

target_sources(ImGui PRIVATE
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h)

target_link_libraries(ImGui PRIVATE glfw)

add_library(Runic SHARED
        src/Platform/Linux/LinuxWindow.cpp
        src/Platform/Linux/LinuxWindow.hpp
        src/Runic.hpp
        src/Runic/Application.cpp
        src/Runic/Application.hpp
        src/Runic/Core.hpp
        src/Runic/Event/ApplicationEvent.hpp
        src/Runic/Event/Event.hpp
        src/Runic/Event/KeyEvent.hpp
        src/Runic/Event/MouseEvent.hpp
        src/Runic/ImGui/ImGuiLayer.cpp
        src/Runic/ImGui/ImGuiLayer.hpp
        src/Runic/Input.hpp
        src/Runic/Layer.cpp
        src/Runic/Layer.hpp
        src/Runic/LayerStack.cpp
        src/Runic/LayerStack.hpp
        src/Runic/Log.cpp
        src/Runic/Log.hpp
        src/Runic/Main.hpp
        src/Runic/Window.hpp
        src/pch.hpp
        src/Platform/Linux/LinuxInput.cpp
        src/Platform/Linux/LinuxInput.hpp)

target_compile_definitions(Runic
        PRIVATE
        RUNIC_PLATFORM_LINUX=""
        RUNIC_BUILD_LIB=""
        GLFW_INCLUDE_NONE="")

target_link_libraries(Runic PUBLIC spdlog::spdlog ${OPENGL_LIBRARIES} glfw glad_gl_core_46 ImGui)

target_include_directories(Runic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(Runic PRIVATE ${OPENGL_INCLUDE_DIR})

target_precompile_headers(Runic PRIVATE src/pch.hpp)

install(TARGETS Runic DESTINATION lib)
install(FILES src/Runic.hpp DESTINATION include)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp"
        EXCLUDE PATTERN "*.cpp"
        EXCLUDE PATTERN "pch.hpp")
