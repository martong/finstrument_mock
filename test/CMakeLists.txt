project(test)

include_directories(.)
file(GLOB SOURCES "*.cpp")
add_library(test_common STATIC ${SOURCES})

add_subdirectory(semi_instrumented)
add_subdirectory(fully_instrumented)
add_subdirectory(instrument_always_inline)
