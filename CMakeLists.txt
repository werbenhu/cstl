cmake_minimum_required(VERSION 3.26)
project(cstl C)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} STATIC 
	sort.c
        array.c
        list.c
        map.c
        stack.c
        queue.c
        map_example.c)
