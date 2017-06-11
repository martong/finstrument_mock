set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
project(compiler-rt)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_library(mock_san SHARED ${SOURCES})
