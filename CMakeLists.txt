cmake_minimum_required(VERSION 3.16)
project(connect4)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -std=c99 -Werror -g")

add_subdirectory(src)