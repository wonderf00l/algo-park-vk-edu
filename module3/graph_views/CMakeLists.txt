cmake_minimum_required(VERSION 3.11)

project(graph CXX)
set (CMAKE_CXX_FLAGS "-Wextra -Wall -Werror -fsanitize=address -std=c++17")

file(GLOB SOURCES "src/*.cpp")

add_library(graph_lib ${SOURCES})

target_include_directories(graph_lib PUBLIC include)

add_executable(${PROJECT_NAME} main.cpp)

add_compile_options(CMAKE_CXX_FLAGS)
target_link_libraries(${PROJECT_NAME} graph_lib)