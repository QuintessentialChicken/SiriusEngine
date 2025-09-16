add_library(Physics
        Physics.cpp
        Physics.h
)

target_include_directories(Physics PRIVATE "${CMAKE_SOURCE_DIR}/Sirius")
