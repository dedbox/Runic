cmake_minimum_required(VERSION 3.31)
project(Sandbox)

set(CMAKE_CXX_STANDARD 23)

add_custom_target(
  copy_shaders
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/" "$<TARGET_FILE_DIR:Sandbox>/shaders"
  COMMENT "Copying shaders if different...")

add_custom_target(
  copy_textures
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/textures/"
    "$<TARGET_FILE_DIR:Sandbox>/textures"
  COMMENT "Copying textures if different...")

add_executable(Sandbox src/Sandbox.cpp)

target_link_libraries(Sandbox PRIVATE Runic)

add_dependencies(Sandbox copy_shaders copy_textures)

install(TARGETS Sandbox DESTINATION bin)
