cmake_minimum_required(VERSION 3.28)
project(Sirius LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(Core)
add_subdirectory(Graphics)
add_subdirectory(Input)
add_subdirectory(Shaders)
add_subdirectory(WindowApp)
