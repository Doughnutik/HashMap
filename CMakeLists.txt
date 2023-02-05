cmake_minimum_required(VERSION 3.24)
project(hashmap)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wfloat-equal -Wshadow -fsanitize=undefined -fsanitize=address
        -fsanitize=bounds -std=c++2a -g")

add_executable(hashmap
        main.cpp
        hash_map.h)

add_executable(testmap
        main.cpp
        test_map.h)

add_executable(test test.cpp)
add_executable(all_test all_test.cpp)
add_executable(check check.cpp hash_map.h)