cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(ListGraph)

project(ex1 CXX)

add_executable(${PROJECT_NAME} src/main.cpp)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Werror -fsanitize=address)

target_link_libraries(${PROJECT_NAME} PRIVATE ListGraph)