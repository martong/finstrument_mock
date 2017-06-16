project(c_language)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -fno-inline-functions -fsanitize=mock")
include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/compiler-rt/include)

add_executable(c_language_test testMain.c)
target_link_libraries(c_language_test mock_san)
add_test(NAME c_language_test COMMAND c_language_test)
