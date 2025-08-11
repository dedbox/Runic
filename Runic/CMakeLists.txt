cmake_minimum_required(VERSION 3.31)
project(Runic)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_library(Runic SHARED
        src/Runic.hpp
        src/Runic/Application.cpp
        src/Runic/Application.hpp
        src/Runic/Core.hpp
        src/Runic/Main.hpp)

target_compile_definitions(Runic
        PRIVATE
        RUNIC_PLATFORM_LINUX=""
        RUNIC_BUILD_LIB="")

target_include_directories(Runic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

install(TARGETS Runic DESTINATION lib)
install(FILES src/Runic.hpp DESTINATION include)
install(FILES
        src/Runic/Application.hpp
        src/Runic/Core.hpp
        src/Runic/Main.hpp
        DESTINATION include/Runic)
