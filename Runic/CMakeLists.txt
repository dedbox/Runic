cmake_minimum_required(VERSION 3.31)
project(Runic)

set(CMAKE_CXX_STANDARD 23)

add_library(Runic STATIC src/Runic.hpp src/Runic/hello.cpp)

target_precompile_headers(Runic PRIVATE <print>)

target_include_directories(Runic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

install(TARGETS Runic DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Runic.hpp DESTINATION include)
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.cpp" EXCLUDE)
