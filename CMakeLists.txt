cmake_minimum_required(VERSION 3.31)
project(carray_project C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(lib)
add_subdirectory(array_test)
add_subdirectory(ring_buffer_test)
add_subdirectory(map_test)


