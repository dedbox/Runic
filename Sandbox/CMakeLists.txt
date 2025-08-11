cmake_minimum_required(VERSION 3.31)
project(Sandbox)

set(CMAKE_CXX_STANDARD 23)

add_executable(Sandbox src/SandboxApp.cpp)

target_compile_definitions(Sandbox PRIVATE RUNIC_PLATFORM_LINUX="")

target_link_libraries(Sandbox PRIVATE Runic)

install(TARGETS Sandbox DESTINATION bin)
