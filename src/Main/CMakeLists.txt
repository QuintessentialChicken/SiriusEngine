add_executable(Main Main.cpp ${CMAKE_SOURCE_DIR}/sirius.rc)
add_dependencies(Main Shaders)
target_link_libraries(Main PRIVATE
        Core
        Pong
        External
        Graphics
        Input
)