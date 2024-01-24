cmake_minimum_required(VERSION 3.23)
project(cppProj)

set(CMAKE_CXX_STANDARD 23)

add_executable(cppProj main.cpp)

add_subdirectory(modules/ModernGL)
target_link_libraries(cppProj mgl)
