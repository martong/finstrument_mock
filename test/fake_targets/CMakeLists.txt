# We set up the modified mock sanitizer compiler here.
# Note, It is not advised to set up the compiler this way.
# See: https://cmake.org/Wiki/CMake_FAQ#How_do_I_use_a_different_compiler.3F
set(CMAKE_CXX_COMPILER "/Users/mg/Work/finstrument_mock/build_rt/../build/bin/clang++")
project(fake_targets)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=mock")
file(GLOB SOURCES "*.cpp")
add_library(fake_targets STATIC ${SOURCES})
