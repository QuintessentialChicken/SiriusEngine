project(WindowApp)

add_executable(WindowApp WinMain.cpp ${CMAKE_SOURCE_DIR}/sirius.rc)
add_dependencies(WindowApp Shaders)
target_link_libraries(WindowApp PRIVATE
        Core
        External
        Graphics
        Input
)
