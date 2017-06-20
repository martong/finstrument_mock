project(fully_instrumented)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=mock")
include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/compiler-rt/include)

add_executable(no_substitution no_substitution.c increment.c)
target_link_libraries(no_substitution mock_san)
add_executable(substitution substitution.c increment.c)
target_link_libraries(substitution mock_san)
