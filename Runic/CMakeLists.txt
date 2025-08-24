cmake_minimum_required(VERSION 3.31)
project(Runic)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

# SDL3 -------------------------------------------------------------------------

FetchContent_Declare(
  SDL
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.20
  GIT_SHALLOW YES)

set(SDL_SHARED NO)
set(SDL_STATIC YES)

FetchContent_MakeAvailable(SDL)

# spdlog -----------------------------------------------------------------------

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.15.3
  GIT_SHALLOW YES)

FetchContent_MakeAvailable(spdlog)

# OpenGL -----------------------------------------------------------------------

find_package(OpenGL REQUIRED)

# Glad -------------------------------------------------------------------------

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v2.0.8
  SOURCE_SUBDIR cmake
  GIT_SHALLOW YES)

FetchContent_MakeAvailable(glad)

glad_add_library(glad_gl_core_46 STATIC LOADER API gl:core=4.6)

# glm --------------------------------------------------------------------------

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
  GIT_SHALLOW YES)

FetchContent_MakeAvailable(glm)

# Runic ------------------------------------------------------------------------

add_library(
  Runic STATIC
  src/Runic/Core/Application.cpp src/Runic/Core/Input.cpp
  src/Runic/Core/LayerManager.cpp src/Runic/Core/Log.cpp
  src/Runic/Core/Window.cpp src/Runic/Renderer/GraphicsContext.cpp)

target_link_libraries(Runic PUBLIC glm::glm)
target_link_libraries(Runic PRIVATE SDL3::SDL3 spdlog::spdlog
                                    ${OPENGL_LIBRARIES} glad_gl_core_46)

target_precompile_headers(
  Runic
  PUBLIC
  <csignal>
  <cstddef>
  <cstdint>
  <format>
  <functional>
  <iterator>
  <list>
  <map>
  <memory>
  <optional>
  <print>
  <ranges>
  <stdexcept>
  <string>
  <typeindex>
  <utility>
  <vector>)

target_include_directories(Runic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(Runic PRIVATE ${OPENGL_INCLUDE_DIR}
                                         ${glad_INCLUDE_DIR} ${glm_INCLUDE_DIR})

install(TARGETS Runic DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Runic.hpp DESTINATION include)
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.cpp" EXCLUDE)

# Unit Tests -------------------------------------------------------------------

enable_testing()

FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG v2.4.12
  GIT_SHALLOW YES)

FetchContent_MakeAvailable(doctest)

add_subdirectory(tests)
