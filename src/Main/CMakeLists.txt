add_executable(Main
        Main.cpp
        ${CMAKE_SOURCE_DIR}/sirius.rc
)
add_dependencies(Main Shaders)

target_link_libraries(Main PRIVATE
        Game
        Engine
)

target_include_directories(Main PRIVATE
        ${CMAKE_SOURCE_DIR}/Sirius
        ${CMAKE_SOURCE_DIR}
)