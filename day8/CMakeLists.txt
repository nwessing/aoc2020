cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -O2 -Werror=implicit-function-declaration -Wno-gnu-compound-literal-initializer -Werror -Wno-sign-compare")

project(day8)

add_executable(day8 main.c)
target_include_directories(day8 PUBLIC ../shared)

