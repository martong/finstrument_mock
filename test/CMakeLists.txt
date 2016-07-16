project(test)

include_directories(.)
include_directories(../compiler-rt/include)
file(GLOB SOURCES "*.cpp")

add_subdirectory(fake_targets)

add_executable(testMain ${SOURCES})
# TODO these should be set able
set(gtest_include_dir /Users/mg/local/googletest/googletest/include)
set(gtest_static_lib /Users/mg/local/googletest/build/googlemock/gtest/libgtest.a)
include_directories(${gtest_include_dir})
target_link_libraries(testMain fake_targets mock_san ${gtest_static_lib})
