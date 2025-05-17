cmake_minimum_required(VERSION 4.0.2)
project(Sirius LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Sirius)
add_subdirectory(Game)
add_subdirectory(Main)
