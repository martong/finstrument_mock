project(fully_instrumented)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-inline-functions -fsanitize=mock -fsanitize=mock_always_inline")
add_definitions(-D_LIBCPP_MOCK_SAN=1)

include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/compiler-rt/include)
file(GLOB SOURCES "*.cpp")

add_executable(always_inline_test ${SOURCES})
target_link_libraries(always_inline_test test_common mock_san gtest)
add_test(NAME always_inline_test COMMAND always_inline_test)
