cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -g -Werror=implicit-function-declaration -Wno-gnu-compound-literal-initializer -Werror -Wno-sign-compare")

project(day17)

add_executable(day17 main.c)
target_include_directories(day17 PUBLIC ../shared)

