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
        src/Runic/Layer.cpp
        src/Runic/Layer.hpp
        src/Runic/LayerStack.cpp
        src/Runic/LayerStack.hpp
        src/Runic/Log.cpp
        src/Runic/Log.hpp
        src/Runic/Main.hpp
        src/Runic/Window.hpp
        src/pch.hpp)

target_compile_definitions(Runic
        PRIVATE
        RUNIC_PLATFORM_LINUX=""
        RUNIC_BUILD_LIB="")

target_link_libraries(Runic PUBLIC spdlog::spdlog ${OPENGL_LIBRARIES} glfw)

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
