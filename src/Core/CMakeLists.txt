file(GLOB_RECURSE SOURCES *.cpp *.h)

add_library(Core ${SOURCES})
target_link_libraries(Core PRIVATE d3d11.lib dxguid.lib D3DCompiler.lib)
target_include_directories(Core PUBLIC .)
