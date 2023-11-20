cmake_minimum_required(VERSION 3.26)
project(cstl C)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(cstl sort.c
        array.c
        list.c
        map.c
        array_test.c)
