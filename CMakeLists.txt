cmake_minimum_required (VERSION 2.6)
project (s2js2i_test C)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -std=c11 -pedantic -Wall")
endif()

add_library(leptjson ./json-parser/leptjson.c)
add_executable(s2js2i_test unit_test.c unit_test.h main.c)
target_link_libraries(s2js2i_test leptjson)
