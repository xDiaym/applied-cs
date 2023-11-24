cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(lab06 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -fsanitize=address,undefined -ggdb)
add_link_options(-fsanitize=address,undefined)
add_executable(${PROJECT_NAME} main.cpp)
