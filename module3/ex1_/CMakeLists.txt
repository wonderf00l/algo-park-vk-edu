cmake_minimum_required(VERSION 3.11)

project(graph CXX)

file(GLOB SOURCES "src/*.cpp")

add_library(graph_lib ${SOURCES})

target_include_directories(graph_lib PUBLIC include)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} graph_lib)