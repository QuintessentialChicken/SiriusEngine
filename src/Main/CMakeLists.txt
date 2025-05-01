add_executable(Main
        Main.cpp ${CMAKE_SOURCE_DIR}/sirius.rc
        GameWorld.cpp
        GameWorld.h)
add_dependencies(Main Shaders)
target_link_libraries(Main PRIVATE
        Core
        External
        Graphics
        Physics
        Input
)