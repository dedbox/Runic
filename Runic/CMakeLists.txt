cmake_minimum_required(VERSION 3.31)
project(Runic)

set(CMAKE_CXX_STANDARD 23)

add_library(Runic SHARED
        src/Runic/library.cpp)

target_include_directories(Runic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

install(TARGETS Runic DESTINATION lib)
install(FILES
        src/Runic/library.hpp
        DESTINATION include/Runic)
