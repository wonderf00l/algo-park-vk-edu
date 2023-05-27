cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)

add_library(IGraph INTERFACE)
target_include_directories(${LIBRARY_OUTPUT_NAME} INTERFACE include)