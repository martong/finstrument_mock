set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
project(compiler-rt)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_library(mock_san SHARED ${SOURCES})
# We must build a shared library to enforce that the linker chose the
# template instantiations which are instantiated in the TU(s) of this library.
add_library(mock_san_st STATIC ${SOURCES})
#set_target_properties(mock_san_st PROPERTIES OUTPUT_NAME mock_san)
