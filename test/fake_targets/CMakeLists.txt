project(fake_targets)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=mock")
file(GLOB SOURCES "*.cpp")
add_library(fake_targets STATIC ${SOURCES})
