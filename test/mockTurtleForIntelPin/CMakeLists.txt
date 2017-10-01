project(MockTurtleForIntelPin)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-inline-functions")
include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/access_private/include)
file(GLOB SOURCES "*.cpp")

add_executable(mock_turtle_pin ${SOURCES})
target_link_libraries(mock_turtle_pin test_common gmock gtest)
