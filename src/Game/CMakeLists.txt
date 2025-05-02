add_library(Game
        GameWorld.cpp
        GameWorld.h
)

target_include_directories(Game PRIVATE
    ${CMAKE_SOURCE_DIR}/Sirius
)
