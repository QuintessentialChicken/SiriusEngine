project(WindowApp)

add_executable(WindowApp WIN32 WinMain.cpp ${CMAKE_SOURCE_DIR}/sirius.rc)
add_dependencies(WindowApp Shaders)
target_link_libraries(WindowApp PRIVATE
        Core
        Graphics
        Input
)
