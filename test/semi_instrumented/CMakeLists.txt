project(semi_instrumented)

include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/compiler-rt/include)
file(GLOB SOURCES "*.cpp")
#file(GLOB SOURCES main.cpp called.cpp RvoidPvoid.cpp RvoidPvoidTest.cpp RvoidPint.cpp RvoidPintTest.cpp)

add_subdirectory(fake_targets)

add_executable(semi_inst_test ${SOURCES})
target_link_libraries(semi_inst_test fake_targets test_common mock_san gtest)
add_test(NAME semi_inst_test COMMAND semi_inst_test)
