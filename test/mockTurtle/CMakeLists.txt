project(fully_instrumented)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-inline-functions -fsanitize=mock")
include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/compiler-rt/include)
include_directories(${CMAKE_SOURCE_DIR}/access_private/include)
file(GLOB SOURCES "*.cpp")

add_executable(mock_turtle ${SOURCES})
target_link_libraries(mock_turtle test_common mock_san gmock gtest)
add_test(NAME mock_turtle COMMAND mock_turtle)
