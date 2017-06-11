project(fully_instrumented)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-inline-functions -fsanitize=mock")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-inline-functions ")
include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/compiler-rt/include)
file(GLOB SOURCES "*.cpp")

add_executable(fully_inst_test ${SOURCES})
target_link_libraries(fully_inst_test test_common mock_san gtest)
add_test(NAME fully_inst_test COMMAND fully_inst_test)
