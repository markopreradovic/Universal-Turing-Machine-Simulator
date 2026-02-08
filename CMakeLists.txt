cmake_minimum_required(VERSION 3.22)
project(utm_simulator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wshadow
        -Wconversion
)

add_executable(utm
        utm-simulator/src/main.c
        utm-simulator/src/tape.c
        utm-simulator/src/tm.h
        utm-simulator/src/tm.c
)

target_include_directories(utm PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/utm-simulator/src
)