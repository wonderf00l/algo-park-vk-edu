cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(../IGraph ListGraphBuild)

add_library(ListGraph SHARED src/ListGraph.cpp)

target_compile_options(${LIBRARY_OUTPUT_NAME} PUBLIC -Wall -Wextra -Werror -fsanitize=address)
target_include_directories(${LIBRARY_OUTPUT_NAME} include)
target_link_libraries(${LIBRARY_OUTPUT_NAME} IGraph)