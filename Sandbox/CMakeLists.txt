cmake_minimum_required(VERSION 3.31)
project(Sandbox)

set(CMAKE_CXX_STANDARD 23)

add_executable(Sandbox src/Sandbox.cpp)

target_link_libraries(Sandbox PRIVATE Runic)

add_custom_command(
  TARGET Sandbox
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
    "$<TARGET_FILE_DIR:Sandbox>/shaders")

install(TARGETS Sandbox DESTINATION bin)
