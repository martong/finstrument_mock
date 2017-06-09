project(fully_instrumented)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-inline-functions -fsanitize=mock")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-inline-functions ")
include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/compiler-rt/include)
file(GLOB SOURCES "*.cpp")

add_executable(fully_inst_test ${SOURCES})

# There is a problem with template instantiations of the used unordered_map in
# the implementation. On OSX the shared library exports the instantiated
# functions to the text segment but on Linux they are weak symbols.
#
# OSX and libc++
# 0000000000004b20 t __ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi
# 0000000000003d00 t __ZNSt3__121__murmur2_or_cityhashImLm64EEclEPKvm
#
# Linux
# 0000000000007350 W __gnu_cxx::new_allocator<std::pair<char* const, char*> >::~new_allocator()
#
# On Linux in case of shared libs linker would choose an instruemented
# definition of the hash function , causing the call of __fake_hook from the
# dtor of the static hash map, that is fatal. A temporary solution is to build
# a static lib under Linux, because that way the correct definition will be
# picked.
# https://software.intel.com/en-us/forums/intel-c-compiler/topic/386159
# https://en.wikipedia.org/wiki/Weak_symbol#Limitations
#
# Hopefully this will not needed soon, when the shadow memory for the functions
# is implemented
message(STATUS ${CMAKE_SYSTEM_NAME})
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_libraries(fully_inst_test test_common mock_san_st gtest)
else()
  target_link_libraries(fully_inst_test test_common mock_san gtest)
endif()

add_test(NAME fully_inst_test COMMAND fully_inst_test)
