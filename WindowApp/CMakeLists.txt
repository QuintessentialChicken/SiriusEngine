project(WindowApp)

file(GLOB_RECURSE SOURCES *.cpp *.h ${CMAKE_SOURCE_DIR}/sirius.rc)
add_executable(WindowApp WIN32 ${SOURCES})

target_link_libraries(WindowApp PRIVATE Core)