project(fully_instrumented)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-inline-functions")
include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/compiler-rt/include)

add_executable(direct no_substitution.c increment.c)
target_link_libraries(direct)
add_executable(viafunp substitution.c increment.c)
target_link_libraries(viafunp)
