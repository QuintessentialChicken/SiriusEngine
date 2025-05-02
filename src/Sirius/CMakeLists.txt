include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(Engine INTERFACE)


target_link_libraries(Engine INTERFACE
        Core
        External
        Graphics
        Input
        Physics
)
add_subdirectory(External)
add_subdirectory(Core)
add_subdirectory(Graphics)
add_subdirectory(Input)
add_subdirectory(Physics)
add_subdirectory(Shaders)


