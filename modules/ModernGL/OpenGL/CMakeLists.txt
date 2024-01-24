cmake_minimum_required(VERSION 3.23)
project(OpenGL C CXX)

set(CMAKE_CXX_STANDARD 23)

add_library(OpenGL include/OpenGL)

add_subdirectory(glad)
add_subdirectory(glfw)

target_link_libraries(OpenGL glad glfw)

target_include_directories(OpenGL PUBLIC include)

set_target_properties(OpenGL PROPERTIES LINKER_LANGUAGE C)
