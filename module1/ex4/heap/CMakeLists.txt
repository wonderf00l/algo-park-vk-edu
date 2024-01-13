cmake_minimum_required(VERSION 3.0)

add_library(heap src/heap.cpp)
target_include_directories(heap PUBLIC include)

target_link_libraries(heap comps)

# add_library(heap_ STATIC src/heap.cpp)
# target_include_directories(heap_ PUBLIC include)
