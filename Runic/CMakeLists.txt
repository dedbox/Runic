cmake_minimum_required(VERSION 3.31)
project(Runic)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
  SDL
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.20
  GIT_SHALLOW YES)

set(SDL_SHARED NO)
set(SDL_STATIC YES)

FetchContent_MakeAvailable(SDL)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.15.3
  GIT_SHALLOW YES)

FetchContent_MakeAvailable(spdlog)

add_library(
  Runic STATIC
  src/Runic/Core.hpp src/Runic/Core/Application.cpp src/Runic/Core/Input.cpp
  src/Runic/Core/LayerManager.cpp src/Runic/Core/Log.cpp
  src/Runic/Core/Window.cpp)

target_link_libraries(Runic PRIVATE SDL3::SDL3 spdlog::spdlog)
# target_link_libraries(Runic INTERFACE spdlog::spdlog)

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

install(TARGETS Runic DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Runic.hpp DESTINATION include)
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.cpp" EXCLUDE)

# Unit Tests ###################################################################

enable_testing()

FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG v2.4.12
  GIT_SHALLOW YES)

FetchContent_MakeAvailable(doctest)

add_subdirectory(tests)
