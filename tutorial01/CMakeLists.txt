cmake_minimum_required(VERSION 3.0)
project(leptjson_test CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

add_library(leptjson leptjson.cpp)
add_executable(leptjson_test test.cpp)
target_link_libraries(leptjson_test leptjson)