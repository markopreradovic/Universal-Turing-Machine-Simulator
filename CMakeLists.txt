cmake_minimum_required(VERSION 3.22)
project(utm_simulator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wshadow -Wconversion)


add_executable(utm
        src/main.c
)
