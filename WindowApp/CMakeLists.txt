project(WindowApp)

file(GLOB_RECURSE SOURCES *.cpp *.h)


add_executable(WindowApp WIN32 ${SOURCES})
