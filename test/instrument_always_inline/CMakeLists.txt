project(fully_instrumented)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-inline-functions -fsanitize=mock -fsanitize=mock_always_inline")
add_definitions(-D_LIBCPP_MOCK_SAN=1)

include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/compiler-rt/include)
file(GLOB SOURCES "*.cpp")

# TODO DRY
add_executable(always_inline_test ${SOURCES})
if (NOT DEFINED gtest_include_dir)
    message(FATAL_ERROR "Please define gtest_include_dir,\
    e.g. with -Dgtest_include_dir=/path/to/googletest/include")
endif()
if (NOT DEFINED gtest_static_lib)
    message(FATAL_ERROR "Please define gtest_static_lib,\
    e.g. with -Dgtest_static_lib=/path/to/googletest/gtest/libgtest.a")
endif()
include_directories(${gtest_include_dir})

target_link_libraries(always_inline_test test_common mock_san ${gtest_static_lib})
