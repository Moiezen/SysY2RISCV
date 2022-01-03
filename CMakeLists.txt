cmake_minimum_required(VERSION 3.5)
project(ztr_compiler LANGUAGES CXX VERSION "0.0.1")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wno-register)

add_executable(compiler
    ${CMAKE_SOURCE_DIR}/main.cpp
)

target_include_directories(compiler
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})