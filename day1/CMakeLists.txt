cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -O2 -Werror=implicit-function-declaration -Wno-gnu-compound-literal-initializer -Werror -Wno-sign-compare")

project(day1)

add_executable(day1 main.c)

