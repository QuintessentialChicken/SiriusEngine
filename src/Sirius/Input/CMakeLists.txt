add_library(Input
        Keyboard.cpp
        Keyboard.h
        Mouse.cpp
        Mouse.h
        Input.cpp
        Input.h
)

target_include_directories(Input PRIVATE "${CMAKE_SOURCE_DIR}/Sirius")


