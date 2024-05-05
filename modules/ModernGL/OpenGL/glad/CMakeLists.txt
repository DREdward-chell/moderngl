cmake_minimum_required(VERSION 3.23)
project(glad C)

set(CMAKE_C_STANDARD 23)

add_library(glad src/glad.c)

target_include_directories(glad PUBLIC include)

set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)
