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

add_library(Runic STATIC src/Runic.hpp src/Runic/Core/Window.cpp)

target_link_libraries(Runic PRIVATE SDL3::SDL3)

target_precompile_headers(
  Runic PRIVATE
  # data types
  <cstdint> <string>)

target_include_directories(Runic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

install(TARGETS Runic DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Runic.hpp DESTINATION include)
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.cpp" EXCLUDE)
