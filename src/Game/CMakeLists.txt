add_library(Game
        App.cpp
        App.h
        GameWorld.cpp
        GameWorld.h
        Game.cpp
        Game.h
)

target_include_directories(Game PRIVATE
    ${CMAKE_SOURCE_DIR}/Sirius
)
