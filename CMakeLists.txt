cmake_minimum_required(VERSION 3.23)
project(moderngl)

set(CMAKE_CXX_STANDARD 23)

add_executable(moderngl main.cpp
        examples/triangle.cpp)

add_subdirectory(modules/ModernGL)
target_link_libraries(moderngl mgl)
