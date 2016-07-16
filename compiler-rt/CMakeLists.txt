project(compiler-rt)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_library(mock_san STATIC ${SOURCES})
