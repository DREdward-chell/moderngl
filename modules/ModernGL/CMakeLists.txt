cmake_minimum_required(VERSION 3.23)
project(mgl)

set(CMAKE_CXX_STANDARD 23)
set(SOURCES  src/mgl_textures.cpp src/mgl_core.cpp include/moderngl)

add_library(mgl ${SOURCES})

add_subdirectory(OpenGL)
target_link_libraries(mgl OpenGL)

target_include_directories(mgl PUBLIC include)
